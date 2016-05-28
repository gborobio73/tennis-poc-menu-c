/**
 * Example implementation of the radio button list UI pattern.
 */

#include "number_of_sets_window.h"
#include "../match/MatchConfiguration.h"

static Window *s_main_window;
static MenuLayer *s_menu_layer;

static int s_current_selection = 0;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return RADIO_BUTTON_WINDOW_NUM_ROWS;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
    // This is a choice item
    static char s_buff[16];    
    if (cell_index->row == 0){
        snprintf(s_buff, sizeof(s_buff), "Best of 3 sets");
    }else{
        snprintf(s_buff, sizeof(s_buff), "Best of 5 sets");
    }
    menu_cell_basic_draw(ctx, cell_layer, s_buff, NULL, NULL);

    GRect bounds = layer_get_bounds(cell_layer);
    GPoint p = GPoint(bounds.size.w - (3 * RADIO_BUTTON_WINDOW_RADIO_RADIUS), (bounds.size.h / 2));

    // Selected?
    if(menu_cell_layer_is_highlighted(cell_layer)) {
        graphics_context_set_stroke_color(ctx, GColorWhite);
        graphics_context_set_fill_color(ctx, GColorWhite);
    } else {
        graphics_context_set_fill_color(ctx, GColorBlack);
    }

    // Draw radio filled/empty
    graphics_draw_circle(ctx, p, RADIO_BUTTON_WINDOW_RADIO_RADIUS);
    
    if(cell_index->row == 0 && match_config_is_best_of_3_sets()== true) {
      // This is the selection
      graphics_fill_circle(ctx, p, RADIO_BUTTON_WINDOW_RADIO_RADIUS - 3);
    }else if(cell_index->row == 1 && match_config_is_best_of_3_sets()== false){
      // This is the selection
      graphics_fill_circle(ctx, p, RADIO_BUTTON_WINDOW_RADIO_RADIUS - 3);
    }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ? 
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    44);
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
    // Change selection
    //s_current_selection = cell_index->row;
    if (cell_index->row ==0)
    {
        match_config_set_best_of_3_sets();
    }else{
        match_config_set_best_of_5_sets();
    }
    menu_layer_reload_data(menu_layer);
    window_stack_pop(true);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);

  window_destroy(window);
  s_main_window = NULL;
}

void number_of_sets_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}
