#include "activity.h"
#include "glibmm/ustring.h"
#include "gtkmm/enums.h"
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <sys/types.h>

DateEventData::DateEventData(){

}
DateEventData::~DateEventData(){

}

ShowActivityWindow::ShowActivityWindow(DateEventData* dateEventDataI)
:   mainBox(Gtk::Orientation::VERTICAL),
    deleteButton("Delete Activity")
{
    dateEventData = dateEventDataI;
    Gtk::Label title;
    title.set_markup("<b>" + dateEventData->text + "</b>");
    mainBox.append(title);
    Gtk::Label bodyLabel;
    bodyLabel.set_wrap(true);
    bodyLabel.set_label(dateEventData->body);
    mainBox.append(bodyLabel);
    deleteButton.signal_clicked().connect( sigc::mem_fun(*this,
              &ShowActivityWindow::onDeleteButtonClicked) );

    mainBox.append(deleteButton);
    set_child(mainBox);
}

ShowActivityWindow::~ShowActivityWindow(){

}

void ShowActivityWindow::onDeleteButtonClicked(){
    dateEventData->deleted = true;
    dateEventData->guiOptions->resetWindow();
    saveActivities(getenv("HOME") + SAVE_FILE_PATH);
    delete this;
}

AddEventWindow::AddEventWindow(GuiOptions* mainWindowI)
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
    bodyEntry(),

    AddButton("Add")
{
    mainWindow = mainWindowI;
    AddButton.signal_clicked().connect( sigc::mem_fun(*this,
              &AddEventWindow::onAddButtonClicked) );

    m_Entry.set_max_length(200);
    m_Entry.set_placeholder_text("Enter activity");
    m_Entry.set_margin(5);

    bodyEntry.set_max_length(1000);
    bodyEntry.set_placeholder_text("Enter body");
    bodyEntry.set_margin(5);

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
    eventSetBox.append(bodyEntry);
    eventSetBox.append(AddButton);
}

AddEventWindow::~AddEventWindow(){

}

void AddEventWindow::onAddButtonClicked(){
    GDate* startEventDate = g_date_new();
    g_date_set_dmy(startEventDate, m_SpinButton_Day.get_value_as_int(), (GDateMonth)m_SpinButton_Month.get_value_as_int(), m_SpinButton_Year.get_value_as_int());
    DateEvent* thisEvent = new DateEvent(startEventDate, m_Entry.get_text(),bodyEntry.get_text() , 1);
    thisEvent->guiOptions = mainWindow;
    eventsArray.push_back(thisEvent);
    saveActivities(getenv("HOME") + SAVE_FILE_PATH);
    mainWindow->resetWindow();
    delete this;
}

DateEvent::DateEvent(GDate* startTimeInput, Glib::ustring textInput,Glib::ustring bodyInput , bool fullDayInput){
    startTime = startTimeInput;
    text = textInput;
    body = bodyInput;
    fullDay = fullDayInput;
    activityButton.set_label(text);
    activityButton.signal_clicked().connect( sigc::mem_fun(*this,
              &DateEvent::onButtonClicked) );
}

DateEvent::~DateEvent(){
    g_date_free(startTime);
    g_date_free(endTime);
}

void DateEvent::onButtonClicked(){
    showActivityWindow = new ShowActivityWindow(this);
    showActivityWindow->show();
}

void saveActivities(std::string path){
    std::ofstream saveFile(path, std::ios_base::trunc);

    if (!saveFile.is_open()) {
        std::cerr << "Error opening save file." << std::endl;
        return;
    }

    uint8_t major = 0;
    uint8_t minor = 1;
    saveFile.write(reinterpret_cast<char*>(&major), sizeof(uint8_t));
    saveFile.write(reinterpret_cast<char*>(&minor), sizeof(uint8_t));

    for(int i = 0; i < eventsArray.size(); i++){
        if(!eventsArray[i]->deleted){
            uint16_t year = g_date_get_year(eventsArray[i]->startTime);
            saveFile.write(reinterpret_cast<char*>(&year), sizeof(uint16_t));
            uint8_t month = (int)g_date_get_month(eventsArray[i]->startTime);
            saveFile.write(reinterpret_cast<char*>(&month), sizeof(uint8_t));
            uint8_t day = g_date_get_day(eventsArray[i]->startTime);
            saveFile.write(reinterpret_cast<char*>(&day), sizeof(uint8_t));
            
            uint16_t textSize = eventsArray[i]->text.length();
            saveFile.write(reinterpret_cast<char*>(&textSize), sizeof(uint16_t));
            saveFile.write(eventsArray[i]->text.c_str(), eventsArray[i]->text.length());

            uint16_t bodySize = sizeof(char)*eventsArray[i]->body.length();
            saveFile.write(reinterpret_cast<char*>(&bodySize), sizeof(uint16_t));
            saveFile.write(eventsArray[i]->body.c_str(), eventsArray[i]->body.length());

            saveFile.write(reinterpret_cast<char*>(&eventsArray[i]->fullDay), sizeof(bool));
        }
    }
    saveFile.close();
    std::cout << "activities saved" << std::endl;
}

void readActivities(std::string path, GuiOptions* guiOptions){
    std::ifstream saveFile(path);

    if (!saveFile.is_open()) {
        std::cerr << "Error opening save file." << std::endl;
        return;
    }
    uint8_t major;
    uint8_t minor;
    
    saveFile.read(reinterpret_cast<char*>(&major), sizeof(uint8_t));
    saveFile.read(reinterpret_cast<char*>(&minor), sizeof(uint8_t));
    std::cout << "opening save file with version: " << (int)major << "." << (int)minor << std::endl;

    while(saveFile.peek() != EOF){
        GDate* startEventDate = g_date_new();
        uint16_t year;
        uint8_t month;
        uint8_t day;
        saveFile.read(reinterpret_cast<char*>(&year), sizeof(uint16_t));
        saveFile.read(reinterpret_cast<char*>(&month), sizeof(uint8_t));
        saveFile.read(reinterpret_cast<char*>(&day), sizeof(uint8_t));
        g_date_set_dmy(startEventDate, (int)day, (GDateMonth)(int)month, (int)year);

        uint16_t tilteSize;
        saveFile.read(reinterpret_cast<char*>(&tilteSize), sizeof(uint16_t));
        char* titleChar = new char[(int)tilteSize];
        saveFile.read(titleChar, (int)tilteSize);
        Glib::ustring title;
        title.assign(titleChar, titleChar+(int)tilteSize);

        uint16_t bodySize;
        saveFile.read(reinterpret_cast<char*>(&bodySize), sizeof(uint16_t));
        char* bodyChar = new char[(int)bodySize];
        saveFile.read(bodyChar, (int)bodySize);
        Glib::ustring body;
        body.assign(bodyChar, bodyChar+(int)bodySize);
        bool isFullDay;
        saveFile.read(reinterpret_cast<char*>(&isFullDay), sizeof(bool));
        DateEvent* thisEvent = new DateEvent(startEventDate, title, body, isFullDay);
        thisEvent->guiOptions = guiOptions;
        eventsArray.push_back(thisEvent);
    }

}