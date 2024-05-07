#pragma once
#include "gtkmm/dialog.h"
#include "gtkmm/window.h"
#include <string>
#include <iostream>
#include <vector>
#include <gtkmm.h>

#define ROWS 6
#define COLUMNS 7

struct Month {
    int id;
    int numDays;
    std::string name;
};

class CalWindow : public Gtk::Window
{
public:
    CalWindow();
    ~CalWindow() override;

protected:
    void changeDays(GDate* shownDate, Gtk::Frame* frames[]);
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
    void onAddFinalEventClicked();
    
    GDate *setDate = g_date_new();

    Glib::RefPtr<Gtk::CssProvider> grayBackground = Gtk::CssProvider::create();
    Glib::ustring grayBackgroundStyle = "* { background-color: #D6D6D6; }";

    Glib::RefPtr<Gtk::CssProvider> defaultCss = Gtk::CssProvider::create();
    Glib::ustring defaultStyle = "* { background-color: #F6F5F4; }";

    Gtk::Window *AddEventDialogP;
    Gtk::Entry* AddeventEntry;

    //Gtk::Dialog AddEventDialog;
};

class DateEvent {
public:
    DateEvent(GDate* startTimeInput, Glib::ustring textInput, bool fullDayInput);
    ~DateEvent();
private:
    Glib::ustring text;
    GDate* startTime;
    GDate* endTime;
    bool fullDay = false;
};