#include <pebble.h>
#include "animated_ab.h"

Window *my_window;
ActionBarLayer *action_bar;

static GBitmap *action_icon_plus;
static GBitmap *action_icon_minus;

static void click_handler(ClickRecognizerRef recognizer, Window *window) {
  Layer *layer = action_bar_layer_get_layer(action_bar);
  
  GRect to_rect = layer_get_bounds(layer);

  switch(click_recognizer_get_button_id(recognizer)) {
	case BUTTON_ID_DOWN: 
	  hide_actionbar(action_bar);
    break;
	default:
  case BUTTON_ID_UP:
	  show_actionbar(action_bar);
    break;
  }
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) click_handler);
}

void handle_init(void) {
  action_icon_plus = gbitmap_create_with_resource(RESOURCE_ID_PLUS);
  action_icon_minus = gbitmap_create_with_resource(RESOURCE_ID_MINUS);
	
  my_window = window_create();
	
  // Initialize the action bar:
  action_bar = action_bar_layer_create();
  // Associate the action bar with the window:
  action_bar_layer_add_to_window(action_bar, my_window);
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(action_bar, click_config_provider);
  
  // Set the icons:
  // The loading the icons is omitted for brevity... See HeapBitmap.
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, action_icon_plus);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, action_icon_minus);
	
  window_stack_push(my_window, true /* Animated */);
}

void handle_deinit(void) {
  action_bar_layer_destroy(action_bar);
  window_destroy(my_window);
  gbitmap_destroy(action_icon_plus);
  gbitmap_destroy(action_icon_minus);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
