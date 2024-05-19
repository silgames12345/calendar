#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <gtkmm.h>

#define ROWS 6
#define COLUMNS 7

#define EVENT_BUFFER_SIZE 256

class GuiOptions {
public:
    virtual void resetWindow() = 0;
};

class DateEvent {
public:
    DateEvent(GDate* startTimeInput, Glib::ustring textInput, bool fullDayInput);
    ~DateEvent();
    Glib::ustring text;
    GDate* startTime;
    GDate* endTime;
    bool fullDay = false;
private:
};

struct Events {
    DateEvent** dateEvents;
    int length = 0;;
};

class AddEventWindow : public Gtk::Window {
public:
    AddEventWindow(Events* eventsI, GuiOptions* mainWindowI);
    ~AddEventWindow();
protected:
    void onAddButtonClicked();

    GuiOptions* mainWindow;

    Events* events;

    Gtk::Box eventSetBox;
    Gtk::Label explainlabel;

    Gtk::Box m_VBox;
    Gtk::Box m_VBox_Day;
    Gtk::Box m_VBox_Month;
    Gtk::Box m_VBox_Year;
    Gtk::Box m_VBox_Accelerated;
    Gtk::Box m_VBox_Value;
    Gtk::Box m_VBox_Digits;

    Gtk::Label m_Label_Day;
    Gtk::Label m_Label_Month;
    Gtk::Label m_Label_Year;

    Glib::RefPtr<Gtk::Adjustment> m_adjustment_day;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment_month;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment_year;

    Gtk::SpinButton m_SpinButton_Day;
    Gtk::SpinButton m_SpinButton_Month;
    Gtk::SpinButton m_SpinButton_Year;

    Gtk::Entry m_Entry;

    Gtk::Button AddButton;
};

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
