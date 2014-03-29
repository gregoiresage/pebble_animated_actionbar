#include <pebble.h>

Window *my_window;
ActionBarLayer *action_bar;

static GBitmap *action_icon_plus;
static GBitmap *action_icon_minus;

static PropertyAnimation *prop_animation;

static void animation_started(Animation *animation, void *data) {
  
}

static void animation_stopped(Animation *animation, bool finished, void *data) {
  
}

static void destroy_property_animation(PropertyAnimation **prop_animation) {
  if (*prop_animation == NULL) {
    return;
  }

  if (animation_is_scheduled((Animation*) *prop_animation)) {
    animation_unschedule((Animation*) *prop_animation);
  }

  property_animation_destroy(*prop_animation);
  *prop_animation = NULL;
}

static void click_handler(ClickRecognizerRef recognizer, Window *window) {
  Layer *layer = action_bar_layer_get_layer(action_bar);
  
  GRect to_rect = layer_get_bounds(layer);

  switch(click_recognizer_get_button_id(recognizer)) {
	case BUTTON_ID_DOWN: 
	  to_rect.origin.y = 3;
      to_rect.origin.x = 144;
      break;
	default:
    case BUTTON_ID_UP:
	  to_rect.origin.y = 3;
      to_rect.origin.x = 144 - 20;
      break;
  }

  destroy_property_animation(&prop_animation);

  prop_animation = property_animation_create_layer_frame(layer, NULL, &to_rect);
  animation_set_duration((Animation*) prop_animation, 400);
  switch (click_recognizer_get_button_id(recognizer)) {
    case BUTTON_ID_UP:
      animation_set_curve((Animation*) prop_animation, AnimationCurveEaseOut);
      break;
	default:
    case BUTTON_ID_DOWN:
      animation_set_curve((Animation*) prop_animation, AnimationCurveEaseIn);
      break;
  }

  /*
   // Exmple animation parameters:

   // Duration defaults to 250 ms
   animation_set_duration(&prop_animation->animation, 1000);

   // Curve defaults to ease-in-out
   animation_set_curve(&prop_animation->animation, AnimationCurveEaseOut);

   // Delay defaults to 0 ms
   animation_set_delay(&prop_animation->animation, 1000);
   */

  animation_set_handlers((Animation*) prop_animation, (AnimationHandlers) {
    .started = (AnimationStartedHandler) animation_started,
    .stopped = (AnimationStoppedHandler) animation_stopped,
  }, NULL /* callback data */);
  animation_schedule((Animation*) prop_animation);
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
  action_bar_layer_set_click_config_provider(action_bar,
                                             click_config_provider);
  // Set the icons:
  // The loading the icons is omitted for brevity... See HeapBitmap.
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, action_icon_plus);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, action_icon_minus);
	
  window_stack_push(my_window, true /* Animated */);
}

void handle_deinit(void) {
  destroy_property_animation(&prop_animation);
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
