#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#ifdef __GNUC__
#include <getopt.h>
#endif
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif
#include <locale.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"

#include "all.h"

//------------------------------------------------------------------//
//                        Base UI primitives                        //
//------------------------------------------------------------------//

extern int errno;

EMPLOYEE *head = NULL;

extern char *__progname;

static inline void usage(void);

extern char *optarg;
extern int optind, opterr, optopt;

int vflag, qflag, cflag, sflag, lflag;

int main(int argc, char *argv[]) {
  // flag/global vars initializers
  vflag = 0, qflag = 0; /* default off => a quieter program    */
  cflag = 0;            /* default off => there will be colors */
  sflag = 1;            /* default on => sync state with disk  */
  lflag = 0;            /* default off => no redirect of stderr */

  char *path_to_disk_state = (char *)malloc(sizeof(char) * PATH_MAX);
  check_alloc(path_to_disk_state);
  char *path_to_log_file = (char *)malloc(sizeof(char) * PATH_MAX);
  check_alloc(path_to_log_file);

  // env vars parsing
  char *no_color = getenv("NO_COLOR");
  int color = 0;

  if (no_color != NULL && no_color[0] != '\0') {
    color = 1;
  }

// fallback to short args parsing when getopt_long is not available
#if !defined(__GNUC__)
  int opt;
  while ((opt = getopt(argc, argv, "hcqsvl:p:")) != -1) {
    switch (opt) {
    case 'c':
      color = true;
      fprintf(stderr, "Info: disabled colors\n");
      break;
    case 'p':
      strcpy(path_to_disk_state, optarg);
      fprintf(stderr, "Info: given path is %s\n", optarg);
      break;
    case 'q':
      qflag = 1;
      break;
    case 'v':
      vflag = 1;
      break;
    case 's':
      sflag = 0;
      break;
    case 'l':
      strcpy(path_to_log_file, optarg);
      break;
    case 'h':
      usage();
      exit(EXIT_SUCCESS);
    default:
      usage();
      exit(EXIT_FAILURE);
    }
  }
#else
  int opt;
  while (1) {
    static struct option long_options[] = {
        {"quiet", no_argument, &qflag, 'q'},
        {"help", no_argument, 0, 'h'},
        {"no-color", no_argument, &cflag, 'c'},
        {"path", required_argument, 0, 'p'},
        {"stateless", no_argument, 0, 's'},
        {"log-file", required_argument, &lflag, 'l'},
        {"verbose", no_argument, &vflag, 'v'},
        {0, 0, 0, 0},
    };

    int option_index = 0;
    opt = getopt_long(argc, argv, "chqsvl:p:", long_options, &option_index);
    if (opt == -1)
      break;

    switch (opt) {
    case 0:
      /* If this option set a flag, do nothing else now. */
      if (long_options[option_index].flag != 0)
        break;
      printf("option %s", long_options[option_index].name);
      if (optarg)
        printf(" with arg %s", optarg);
      printf("\n");
      break;

    case 'q':
      qflag = 1;
      break;

    case 'c':
      cflag = 0;
      break;

    case 'p':
      strcpy(path_to_disk_state, optarg);
      break;

    case 's':
      sflag = 0;
      break;

    case 'l':
      lflag = 1;
      strcpy(path_to_log_file, optarg);
      break;

    case 'h':
      usage();
      exit(EXIT_SUCCESS);

    case 'v':
      vflag = 1;
      break;

    case '?':
      /* getopt_long already printed an error message. */
      break;

    default:
      fprintf(stderr, "Error: unexpected failure while reading cli args\n");
      abort();
    }
  }
#endif

  // default state file to "state/record.txt"
  if (path_to_disk_state[0] == '\0')
    strcpy(path_to_disk_state, "state/record.txt");

  // redirect stderr to supplied path
  if (lflag) {
    FILE *Fp = freopen(path_to_log_file, "a+", stderr);
    if (Fp == NULL) {
      panic("Fatal: failed redirection of stderr");
    }
  }

  // warn on extraneous args
  if (optind < argc) {
    fprintf(stderr, "Warn: extra unused argv elements\n");
    while (optind < argc)
      fprintf(stderr, "%s ", argv[optind++]);
    fprintf(stderr, "\n");
  }

  vflag &= !qflag;
  cflag |= color;

  // general loging
  if (vflag) {
    // TODO: add miscellaneous info here
    fprintf(stderr, "Info: verbose mode is on\n");
    if (cflag)
      fprintf(stderr, "Info: color mode is on\n");
    if (!sflag)
      fprintf(stderr, "Info: state file at '%s'\n", path_to_disk_state);
    else
      fprintf(stderr, "Info: saving state to disk is disabled\n");
  }

  //------------------------------------------------------------------//
  //           Initializing nuklear, SDL and EMPLOYEE list            //
  //------------------------------------------------------------------//

  /* Platform */
  SDL_Window *win;
  SDL_Renderer *renderer;
  int running = 1;
  uint32_t flags = 0;
  float font_scale = 1;

  /* GUI */
  struct nk_context *ctx;
  struct nk_colorf bg;

  /* SDL setup */
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
  SDL_Init(SDL_INIT_VIDEO);

  win = SDL_CreateWindow("Employee Management", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                         SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

  if (win == NULL) {
    SDL_Log("Error SDL_CreateWindow %s", SDL_GetError());
    exit(-1);
  }

  flags |= SDL_RENDERER_ACCELERATED;
  flags |= SDL_RENDERER_PRESENTVSYNC;

  renderer = SDL_CreateRenderer(win, -1, flags);

  if (renderer == NULL) {
    SDL_Log("Error SDL_CreateRenderer %s", SDL_GetError());
    exit(-1);
  }

  /* scale the renderer output for High-DPI displays */
  {
    int render_w, render_h;
    int window_w, window_h;
    float scale_x, scale_y;
    SDL_GetRendererOutputSize(renderer, &render_w, &render_h);
    SDL_GetWindowSize(win, &window_w, &window_h);
    scale_x = (float)(render_w) / (float)(window_w);
    scale_y = (float)(render_h) / (float)(window_h);
    SDL_RenderSetScale(renderer, scale_x, scale_y);
    font_scale = scale_y;
  }

  /* GUI */
  ctx = nk_sdl_init(win, renderer);
  /* Load Fonts: default font will be used  */
  {
    struct nk_font_atlas *atlas;
    struct nk_font_config config = nk_font_config(0);
    struct nk_font *font;

    /* set up the font atlas and add desired font; note that font sizes are
     * multiplied by font_scale to produce better results at higher DPIs */
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
    nk_sdl_font_stash_end();

    /* this hack makes the font appear to be scaled down to the desired
     * size and is only necessary when font_scale > 1 */
    font->handle.height /= font_scale;
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    nk_style_set_font(ctx, &font->handle);
  }

  bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
  while (running) {
    /* Input */
    SDL_Event evt;
    nk_input_begin(ctx);
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT)
        goto cleanup;
      nk_sdl_handle_event(&evt);
    }
    nk_input_end(ctx);

    //------------------------------------------------------------------//
    //                            home menu                             //
    //------------------------------------------------------------------//

    /* GUI */
    if (nk_begin(ctx, "Settings", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
      enum { EASY, HARD };
      static int op = EASY;
      static int property = 20;

      nk_layout_row_static(ctx, 30, 80, 1);
      if (nk_button_label(ctx, "button"))
        fprintf(stdout, "button pressed\n");
      nk_layout_row_dynamic(ctx, 30, 2);
      if (nk_option_label(ctx, "easy", op == EASY))
        op = EASY;
      if (nk_option_label(ctx, "hard", op == HARD))
        op = HARD;
      nk_layout_row_dynamic(ctx, 25, 1);
      nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

      nk_layout_row_dynamic(ctx, 20, 1);
      nk_label(ctx, "background:", NK_TEXT_LEFT);
      nk_layout_row_dynamic(ctx, 25, 1);
      if (nk_combo_begin_color(ctx, nk_rgb_cf(bg),
                               nk_vec2(nk_widget_width(ctx), 400))) {
        nk_layout_row_dynamic(ctx, 120, 1);
        bg = nk_color_picker(ctx, bg, NK_RGBA);
        nk_layout_row_dynamic(ctx, 25, 1);
        bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
        bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
        bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
        bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
        nk_combo_end(ctx);
      }
    }
    nk_end(ctx);

    //------------------------------------------------------------------//
    //                         main menu logic                          //
    //------------------------------------------------------------------//

    if (nk_begin(ctx, "main menu", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
    }
    nk_end(ctx);

    // TODO: add main menu
    const char *choices_main[5] = {
        "Adding employee record",
        "Delete employee record",
        "Modify employee field ",
        "Search for employee by field",
        "Quit",
    };

    //------------------------------------------------------------------//
    //                         operations menu                          //
    //------------------------------------------------------------------//

    // TODO: add search menu
    const char *choices_search[3] = {
        "By Identifier",
        "By Name",
        "Go back to main menu",
    };

    //------------------------------------------------------------------//
    //                           fuzzy search                           //
    //------------------------------------------------------------------//

    // TODO: add fuzzy search

    //------------------------------------------------------------------//
    //                         resource cleanup                         //
    //------------------------------------------------------------------//

    SDL_SetRenderDrawColor(renderer, (Uint8)(bg.r * 255), (Uint8)(bg.g * 255),
                           (Uint8)(bg.b * 255), (Uint8)(bg.a * 255));
    SDL_RenderClear(renderer);

    nk_sdl_render(NK_ANTI_ALIASING_ON);

    SDL_RenderPresent(renderer);
  }
  // NOTE: more cleanup can be done

cleanup:
  free(path_to_disk_state);
  free(path_to_log_file);

  nk_sdl_shutdown();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  SDL_Quit();

  return EXIT_SUCCESS;
}

static inline void usage(void) {
  fprintf(stderr,
          "%s - Manage employees\n"
          "usage: %s [-p <path>] [-c] [-v] [-h]\n"
          "\t-c, --no-color       disable color\n"
          "\t-v, --verbose        enable verbose logging\n"
          "\t-q, --quiet          disable verbose logging\n"
          "\t-p, --path PATH      specify path of the state file\n"
          "\t-s, --stateless      disable use of a state file\n"
          "\t-h, --help           prints out this help\n",
          __progname, __progname);
}
