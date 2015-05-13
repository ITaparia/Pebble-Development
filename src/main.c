#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_normal_layer;
static TextLayer *s_HEY_layer;
static TextLayer *date_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static GFont s_time_font;
static GFont s_HEY_font;
static GFont s_date_font;


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char date_buffer[10];

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  //Write the date into buffer variable
  strftime(date_buffer, sizeof(date_buffer), "%b %e", tick_time);
  
  //Display date text on watch
	text_layer_set_text(date_layer, date_buffer);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(-32, 110, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  
  //Create normal TextLayer
  s_normal_layer = text_layer_create(GRect(38,110,139,50));
  text_layer_set_background_color(s_normal_layer, GColorClear);
  text_layer_set_text_color(s_normal_layer, GColorWhite);
  text_layer_set_text(s_normal_layer, "It\'s ");
  
  //Create HEY TextLayer
  s_HEY_layer = text_layer_create(GRect(3.5,63,139,50));
  text_layer_set_background_color(s_HEY_layer, GColorClear);
  text_layer_set_text_color(s_HEY_layer, GColorWhite);
  text_layer_set_text(s_HEY_layer, "HEY!");
  
  //Create date TextLayer
  date_layer = text_layer_create(GRect(0, 130, 144, 20));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DUAL_15));
  s_HEY_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DUAL_35));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DUAL_15));

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
  text_layer_set_font(s_normal_layer, s_time_font);
  text_layer_set_text_alignment(s_normal_layer, GTextAlignmentLeft);
  text_layer_set_font(s_HEY_layer, s_HEY_font);
  text_layer_set_text_alignment(s_HEY_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, s_date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_normal_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_HEY_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_normal_layer);
  text_layer_destroy(s_HEY_layer);
  text_layer_destroy(date_layer);
  
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_HEY_font);

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
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
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}