#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <gtkmm.h>
#include "activity.h"

#define ROWS 6
#define COLUMNS 7

#define EVENT_BUFFER_SIZE 256

struct Month {
    int id;
    int numDays;
    std::string name;
};

class CalWindow : public Gtk::Window, public GuiOptions
{
public:
    CalWindow();
    ~CalWindow() override;
    GDate *setDate = g_date_new();
    void changeDays(GDate* shownDate, Gtk::Frame* frames[]);
    virtual void resetWindow() override;
protected:
    Events events;
    Gtk::Frame* frames[COLUMNS*ROWS];
    Gtk::Label monthAndYear;

    Gtk::Box mainBox;
    Gtk::Box buttonBox;
    Gtk::Grid mainGrid;

    Gtk::Button prevButton;
    void onPrevClicked();
    Gtk::Button nextButton;
    void onNextClicked();
    Gtk::Button addEventButton;
    void onAddEventClicked();

    Glib::RefPtr<Gtk::CssProvider> grayBackground = Gtk::CssProvider::create();
    Glib::ustring grayBackgroundStyle = "* { background-color: #D6D6D6; }";

    Glib::RefPtr<Gtk::CssProvider> defaultCss = Gtk::CssProvider::create();
    Glib::ustring defaultStyle = "* { background-color: #F6F5F4; }";

    Gtk::Window *AddEventDialogP;
    Gtk::Entry* AddeventEntry;

    AddEventWindow* addEventWindow = NULL;

    //Gtk::Dialog AddEventDialog;
};
