#include "gui.h"
#include "activity.h"
#include <cstddef>

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

    //get activities 
    readActivities(getenv("HOME") + SAVE_FILE_PATH, this);

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
            for(int i = 0; i < eventsArray.size(); i++){
                eventIndex = i;
                int eventYear = g_date_get_year(eventsArray[i]->startTime);
                int eventMonth = g_date_get_month(eventsArray[i]->startTime);
                int eventDay = g_date_get_day(eventsArray[i]->startTime);
                if(eventYear == year && eventMonth - 1 == monthHolder && eventDay == day && !eventsArray[i]->deleted){
                    eventsArray[i]->activityButton.unparent();
                    activityBox.append(eventsArray[i]->activityButton);
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
    addEventWindow = new AddEventWindow(this);
    addEventWindow->show();
};

void CalWindow::onEventClicked(){
    std::cout << "event clicked with index: " << std::endl;
}