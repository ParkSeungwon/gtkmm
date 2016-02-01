#include <gtkmm.h>
#include <iostream>
#include <sstream>

class mywindow2 : public Gtk::Window
{
    public:
        mywindow2();
        virtual ~mywindow2();
    protected:
    private:
};

class mywindow1 : public Gtk::Window
{
    public:
        mywindow1();
        virtual ~mywindow1();
    protected:
    private:
        int windowcnt = 0;

    struct mypopup
    {
        int windowid;
        mywindow2 *popup;
    };
    void on_b1_click();
    void on_popup_close(mypopup *popup);
    std::map<int, mypopup*> popups;
};

mywindow1::mywindow1()
{
    set_default_size(400, 200);
    set_title("Tutorial 6");


    Gtk::Label *b1label =Gtk::manage(new Gtk::Label);
    b1label->set_markup("<b><i><span color='blue'>Open Popup</span></i></b>");
    Gtk::Button *b1 = Gtk::manage(new Gtk::Button);
    b1->add(*b1label);
    b1->signal_clicked().connect(sigc::mem_fun(*this, &mywindow1::on_b1_click));

    add(*b1);

    show_all_children();
}

mywindow1::~mywindow1()
{
    std::map<int, mypopup*>::iterator iter;
    for (iter = popups.begin(); iter != popups.end(); iter++)
    {
        mypopup *popup = iter->second;

        std::stringstream ss;
        ss << "Deleting Window '" << iter->first << "'";
        std::cout << ss.str() << std::endl;

        delete popup->popup;
        delete popup;
    }
}

void mywindow1::on_b1_click()
{
    mypopup *popup = new mypopup;
    popup->popup = new mywindow2;
    popup->windowid = windowcnt;

    std::stringstream ss;
    ss << "Tutorial 6 - New Popup '" << windowcnt << "'";
    popup->popup->set_title(ss.str());
    popup->popup->signal_hide().connect(sigc::bind<mypopup*>(sigc::mem_fun(*this, &mywindow1::on_popup_close), popup));

    popups[windowcnt] = popup;
    windowcnt++;

    popup->popup->show();
}

void mywindow1::on_popup_close(mypopup *popup)
{
    std::stringstream ss;

    ss << "Deleting Window '" << popup->windowid << "'";
    std::cout << ss.str() << std::endl;

    popups.erase(popup->windowid);
    delete popup->popup;
    delete popup;
}

mywindow2::mywindow2()
{
    set_default_size(400, 200);

    Gtk::Label *label = Gtk::manage(new Gtk::Label("New Popup Window"));

    add(*label);

    show_all_children();
}

mywindow2::~mywindow2()
{
    //dtor
}

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "com.gtkmm.tutorial6.base");

    mywindow1 window;

    return app->run(window);
}

