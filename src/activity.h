#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <gtkmm.h>

inline std::string SAVE_FILE_PATH =  "/.local/share/calendar/activities.af";

class GuiOptions {
public:
    virtual void resetWindow() = 0;
};

class DateEventData{
public:
    DateEventData();
    ~DateEventData();
    Glib::ustring text;
    Glib::ustring body;
    bool deleted = false;
    GuiOptions* guiOptions = NULL;
};

class ShowActivityWindow : public Gtk::Window {
public:
    ShowActivityWindow(DateEventData* dateEventDataI);
    ~ShowActivityWindow();
private:
    Gtk::Button deleteButton;
    Gtk::Box mainBox;
    DateEventData* dateEventData;
    void onDeleteButtonClicked();
};

inline ShowActivityWindow* showActivityWindow = NULL;

class DateEvent : public DateEventData {
public:
    DateEvent(GDate* startTimeInput, Glib::ustring textInput, Glib::ustring bodyInput, bool fullDayInput);
    ~DateEvent();
    GDate* startTime;
    GDate* endTime;
    bool fullDay = false;
    Gtk::Button activityButton;
private:
    void onButtonClicked();
};

inline std::vector<DateEvent*> eventsArray;

class AddEventWindow : public Gtk::Window {
public:
    AddEventWindow(GuiOptions* mainWindowI);
    ~AddEventWindow();
protected:
    void onAddButtonClicked();

    GuiOptions* mainWindow;

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
    Gtk::Entry bodyEntry;

    Gtk::Button AddButton;
};

void saveActivities(std::string path);
void readActivities(std::string path);
