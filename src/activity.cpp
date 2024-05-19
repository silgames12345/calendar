#include "activity.h"

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