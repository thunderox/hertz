CC = g++

# compiler flags:
CFLAGS  = -g -Wall -ljack

# the build target executable:
TARGET = hertz
DELIRIUM_UI = delirium_ui/delirium_ui.cpp delirium_ui/delirium_ui_widget_base.cpp delirium_ui/delirium_ui_widget_adsr.cpp delirium_ui/delirium_ui_widget_button.cpp delirium_ui/delirium_ui_widget_fader.cpp delirium_ui/delirium_ui_widget_fader_route.cpp delirium_ui/delirium_ui_widget_filter.cpp delirium_ui/delirium_ui_widget_knob.cpp delirium_ui/delirium_ui_widget_label.cpp delirium_ui/delirium_ui_widget_list.cpp delirium_ui/delirium_ui_widget_logo.cpp delirium_ui/delirium_ui_widget_panel.cpp delirium_ui/delirium_ui_widget_selector.cpp delirium_ui/delirium_ui_widget_switch.cpp delirium_ui/delirium_ui_widget_tabbed_navigator.cpp
 
all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp song.cpp jack_manager.cpp window.cpp $(DELIRIUM_UI) `pkg-config --cflags --libs jack xcb cairo` 

clean:
	$(RM) $(TARGET)
