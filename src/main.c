#include <pebble.h>
#define PERSIST_INT 1
	
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_letter_layer;
char *current_letter = "Y";
int letter_int = 1;


static void update_day() {
	if (letter_int == 1) {
		current_letter = "A";
		//letter_int++;
	}
	else if (letter_int == 2) {
		current_letter = "B";
		//letter_int++;
	}
	else if (letter_int == 3) {
		current_letter = "C";
		//letter_int++;
	}
	else if (letter_int == 4) {
		current_letter = "D";
		//letter_int++;
	}
	else if (letter_int == 5) {
		current_letter = "E";
		//letter_int++;
	}
	else if (letter_int == 6) {
		current_letter = "F";
		//letter_int = 1;
	}
	else {
		current_letter = "ERR";
	}
	text_layer_set_text(s_letter_layer, current_letter);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
	//static char letter_buffer[] = "X";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 24h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
	
	s_letter_layer = text_layer_create(GRect(0, 5, 144, 30));
	text_layer_set_background_color(s_letter_layer, GColorClear);
  text_layer_set_text_color(s_letter_layer, GColorBlack);
	text_layer_set_text(s_letter_layer, "Loading...");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	
	text_layer_set_font(s_letter_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_letter_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_letter_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
	text_layer_destroy(s_letter_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void tick_handler_days(struct tm *tick_time, TimeUnits units_changed) {
  update_day();
	
	
	//Checking if its a new day
	static char day_check[] = "01";
	strftime(day_check, sizeof("01"), "%H", tick_time);
	if (strcmp(day_check, "00") == 0) {
		letter_int++;
	}
}
  
static void init() {
	if (persist_exists(PERSIST_INT)) {
    // Load stored count
    letter_int = persist_read_int(PERSIST_INT);
  }
	
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_days);
	
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
	persist_write_int(PERSIST_INT, letter_int);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
