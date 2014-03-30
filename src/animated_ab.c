#include <pebble.h>

static PropertyAnimation *prop_animation;

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

static void animation_stopped(Animation *animation, bool finished, void *data) {
	destroy_property_animation(&prop_animation);
}

static void move_actionbar(ActionBarLayer *action_bar, bool in){
	Layer *layer = action_bar_layer_get_layer(action_bar);

	GRect to_rect = layer_get_bounds(layer);
	to_rect.origin.y = 3;

	if(in){
		to_rect.origin.x = 144 - 20;
	}
	else {
		to_rect.origin.x = 144;
	}

	destroy_property_animation(&prop_animation);

	prop_animation = property_animation_create_layer_frame(layer, NULL, &to_rect);
	animation_set_duration((Animation*) prop_animation, 400);
	if(in){
		animation_set_curve((Animation*) prop_animation, AnimationCurveEaseIn);
	}
	else {
		animation_set_curve((Animation*) prop_animation, AnimationCurveEaseOut);
	}

	animation_set_handlers((Animation*) prop_animation, (AnimationHandlers) {
		.stopped = (AnimationStoppedHandler) animation_stopped,
  		}, NULL /* callback data */);
	animation_schedule((Animation*) prop_animation);
}

void show_actionbar(ActionBarLayer *action_bar){
	move_actionbar(action_bar, true);
}

void hide_actionbar(ActionBarLayer *action_bar){
	move_actionbar(action_bar, false);
}