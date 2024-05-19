#include "gui.h"
#include "gtkmm/adjustment.h"
#include "gtkmm/button.h"
#include "gtkmm/dialog.h"
#include "gtkmm/enums.h"
#include "gtkmm/label.h"
#include "gtkmm/spinbutton.h"
#include "gtkmm/window.h"
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

std::string monthNames[12]
    = { "Jan", 
        "Feb", 
        "Mar", 
        "Apr", 
        "May", 
        "Jun", 
        "Jul", 
        "Aug", 
        "Sep", 
        "Oct", 
        "Nov", 
        "Dec" };

CalWindow::CalWindow()
:   mainBox(Gtk::Orientation::VERTICAL),
    buttonBox(),
    nextButton(">"),
    prevButton("<"),
    addEventButton("+"),
    monthAndYear("<b>test</b>")
{   
    events.dateEvents = new DateEvent*[EVENT_BUFFER_SIZE];

    set_title("Calendar");
    set_default_size(1000, 500);

    g_date_set_time_t(setDate, time(NULL));

    set_child(mainBox);
    mainBox.append(buttonBox);
    mainBox.append(mainGrid);

    grayBackground->load_from_data(grayBackgroundStyle);
    defaultCss->load_from_data(defaultStyle);

    prevButton.signal_clicked().connect( sigc::mem_fun(*this,
              &CalWindow::onPrevClicked) );
    nextButton.signal_clicked().connect( sigc::mem_fun(*this,
              &CalWindow::onNextClicked) );
    addEventButton.signal_clicked().connect( sigc::mem_fun(*this,
              &CalWindow::onAddEventClicked) );
    buttonBox.append(prevButton);
    buttonBox.append(nextButton);
    buttonBox.append(monthAndYear);
    buttonBox.append(addEventButton);

    mainGrid.set_expand(true);
    mainGrid.set_row_homogeneous(true);
    mainGrid.set_column_homogeneous(true);
    
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            Gtk::Frame* frame = Gtk::make_managed<Gtk::Frame>();
            mainGrid.attach(*frame, col, row, 1, 1);
            frames[row*COLUMNS+col] = frame;
        }
    }

    //get current date
    changeDays(setDate, frames);
}

CalWindow::~CalWindow(){
g_date_free(setDate);
}

void CalWindow::changeDays(GDate* shownDate, Gtk::Frame* frames[]){
    int year = g_date_get_year(shownDate);
    int currentMonth = g_date_get_month(shownDate);
    int daysInMonth = g_date_get_days_in_month((GDateMonth)currentMonth, g_date_get_year(shownDate));
    int daysInPrevMonth = g_date_get_days_in_month((GDateMonth)(currentMonth == 1 ? 12 : currentMonth-1), g_date_get_year(shownDate));
    GDate *firstDayOfMonth = g_date_new();
    g_date_set_dmy(firstDayOfMonth, 1, (GDateMonth)currentMonth, year);
    int firstDay = g_date_get_weekday(firstDayOfMonth);

    int prevDay = daysInPrevMonth-firstDay+1;

    //fil in grid
    int monthHolder = currentMonth == 1 ? 11 : currentMonth-2;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            Gtk::Frame* frame = frames[row*COLUMNS+col];
            int day = prevDay+1;
            if(row == 0){
                if(day > daysInPrevMonth){
                    day = 1;
                    monthHolder = monthHolder == 11 ? 0 : monthHolder + 1;
                }
            } else if(day > daysInMonth){
                day = 1;
                monthHolder = monthHolder == 11 ? 0 : monthHolder + 1;
            }
            if(monthHolder + 1 == currentMonth){
                frame->get_style_context()->add_provider(defaultCss, GTK_STYLE_PROVIDER_PRIORITY_USER);
            } else {
                frame->get_style_context()->add_provider(grayBackground, GTK_STYLE_PROVIDER_PRIORITY_USER);
            }
            frame->set_label(monthNames[monthHolder]+" "+std::to_string(day));
            Gtk::Box activityBox(Gtk::Orientation::VERTICAL);
            frame->set_child(activityBox);
            for(int i = 0; i < events.length; i++){
                int eventYear = g_date_get_year(events.dateEvents[i]->startTime);
                int eventMonth = g_date_get_month(events.dateEvents[i]->startTime);
                int eventDay = g_date_get_day(events.dateEvents[i]->startTime);
                if(eventYear == year && eventMonth - 1 == monthHolder && eventDay == day){
                    Gtk::Button activityButton(events.dateEvents[i]->text);
                    activityBox.append(activityButton);
                }
            }
            prevDay = day;
        }
    }
    monthAndYear.set_markup("<b>" + std::to_string(year) + " " + monthNames[currentMonth-1] + "</b>");
}

void CalWindow::resetWindow(){
    changeDays(setDate, frames);
}

void CalWindow::onPrevClicked(){
    int year = g_date_get_year(setDate);
    int Month = g_date_get_month(setDate)-1;
    if(Month == 0){
        Month = 12;
        year -= 1;
    }
    setDate = g_date_new_dmy(1, (GDateMonth)Month, year);
    changeDays(setDate, frames);
}

void CalWindow::onNextClicked(){
    int year = g_date_get_year(setDate);
    int Month = g_date_get_month(setDate)+1;
    if(Month == 13){
        Month = 1;
        year += 1;
    }
    setDate = g_date_new_dmy(1, (GDateMonth)Month, year);
    changeDays(setDate, frames);
}

void CalWindow::onAddEventClicked(){  
    addEventWindow = new AddEventWindow(&events, this);
    addEventWindow->show();
};

AddEventWindow::AddEventWindow(Events* eventsI, GuiOptions* mainWindowI)
:   eventSetBox(Gtk::Orientation::VERTICAL, 5),
    explainlabel("Enter date and time of event below."),
    m_VBox(Gtk::Orientation::HORIZONTAL),
    m_VBox_Day(Gtk::Orientation::VERTICAL),
    m_VBox_Month(Gtk::Orientation::VERTICAL),
    m_VBox_Year(Gtk::Orientation::VERTICAL),
    m_VBox_Accelerated(Gtk::Orientation::VERTICAL),
    m_VBox_Value(Gtk::Orientation::VERTICAL),
    m_VBox_Digits(Gtk::Orientation::VERTICAL),

    m_Label_Day("Day: ", Gtk::Align::START),
    m_Label_Month("Month: ", Gtk::Align::START),
    m_Label_Year("Year: ", Gtk::Align::START),

    m_adjustment_day( Gtk::Adjustment::create(1.0, 1.0, 31.0, 1.0, 5.0, 0.0) ),
    m_adjustment_month( Gtk::Adjustment::create(1.0, 1.0, 12.0, 1.0, 5.0, 0.0) ),
    m_adjustment_year( Gtk::Adjustment::create(2024.0, 1.0, 2200.0, 1.0, 100.0, 0.0) ),

    m_SpinButton_Day(m_adjustment_day),
    m_SpinButton_Month(m_adjustment_month),
    m_SpinButton_Year(m_adjustment_year),

    m_Entry(),

    AddButton("Add")
{
    events = eventsI;
    mainWindow = mainWindowI;
    AddButton.signal_clicked().connect( sigc::mem_fun(*this,
              &AddEventWindow::onAddButtonClicked) );

    m_Entry.set_max_length(200);
    m_Entry.set_text("Enter activity");
    m_Entry.set_margin(5);

    AddButton.set_margin(5);

    m_VBox_Day.set_margin(5);
    m_VBox_Day.append(m_Label_Day);
    m_VBox_Day.append(m_SpinButton_Day);

    m_VBox_Month.set_margin(5);
    m_VBox_Month.append(m_Label_Month);
    m_VBox_Month.append(m_SpinButton_Month);

    m_VBox_Year.set_margin(5);
    m_VBox_Year.append(m_Label_Year);
    m_VBox_Year.append(m_SpinButton_Year);

    m_VBox.append(m_VBox_Day);
    m_VBox.append(m_VBox_Month);
    m_VBox.append(m_VBox_Year);

    set_child(eventSetBox);

    eventSetBox.append(explainlabel);
    eventSetBox.append(m_VBox);
    eventSetBox.append(m_Entry);
    eventSetBox.append(AddButton);
}

AddEventWindow::~AddEventWindow(){

}

void AddEventWindow::onAddButtonClicked(){
    GDate* startEventDate = g_date_new();
    g_date_set_dmy(startEventDate, m_SpinButton_Day.get_value_as_int(), (GDateMonth)m_SpinButton_Month.get_value_as_int(), m_SpinButton_Year.get_value_as_int());
    events->dateEvents[events->length] = new DateEvent(startEventDate, m_Entry.get_text(), 1);
    events->length += 1;
    mainWindow->resetWindow();
    delete this;
}

DateEvent::DateEvent(GDate* startTimeInput, Glib::ustring textInput, bool fullDayInput){
    startTime = startTimeInput;
    text = textInput;
    fullDay = fullDayInput;
}