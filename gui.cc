/*
 * gui.cc
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 12
 * 50% Massimiliano 50% Carl
 */


#include "gui.h"
#include <cairomm/context.h>
#include <iostream> 

#include "graphic_gui.h"
#include "graphic.h"
#include "shape.h"
#include "lifeform.h"
#include "simulation.h"

using namespace std;


static bool spawnAlgae = false;

static void orthographicProjection(const Cairo::RefPtr<Cairo::Context>& cr, 
									const Frame& frame);

constexpr unsigned drawingSize(500); 

static Frame default_frame = {0, 256., -0., 256., 1, 
                              drawingSize, drawingSize};



MyArea::MyArea()
{
  setFrame(default_frame);
  set_content_width(default_frame.width);
	set_content_height(default_frame.height);
  set_draw_func(sigc::mem_fun(*this, &MyArea::onDraw));
}

MyArea::~MyArea()
{
}

void MyArea::setFrame(Frame f)
{
	if((f.xMin <= f.xMax) and (f.yMin <= f.yMax) and (f.height > 0))
	{
		f.asp = f.width/f.height;
		frame = f;
	}
	else
		std::cout << "incorrect Model framing or window parameters" << std::endl;
} 

void MyArea::adjustFrame(int width, int height)
{	
	frame.width  = width;
	frame.height = height;

    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp)
    {
	    frame.yMax = default_frame.yMax ;
	    frame.yMin = default_frame.yMin ;	
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);
	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;		  	  
    }
    else
    { 
	    frame.xMax = default_frame.xMax ;
	    frame.xMin = default_frame.xMin ;
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;		  	  
    }
}

void MyArea::setSimulation(Simulation& simulation) {
  sim = &simulation;
  }

void MyArea::onDraw(const Cairo::RefPtr<Cairo::Context>& cr,
                     int width, int height)
{
  graphic_set_context(cr);
	adjustFrame(width,height);
	orthographicProjection(cr, frame); 
  graphicDrawRect(dmax/2,dmax/2,dmax,dmax, GREY);
  drawSimulation((*sim));
}

static void orthographicProjection(const Cairo::RefPtr<Cairo::Context>& cr, 
									const Frame& frame)
{
	cr->translate(frame.width/2, frame.height/2);
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
             -frame.height/(frame.yMax - frame.yMin));
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

Gui::Gui(Simulation& simulation):
  counter(0),
  timerAdded(false),
  disconnect(false),
  timeoutValue(50),
  sim(simulation),
  mainBox(Gtk::Orientation::HORIZONTAL, 5),
  interactBox(Gtk::Orientation::VERTICAL, 15),
  btnBox(Gtk::Orientation::VERTICAL, 5),
  infoBox(Gtk::Orientation::VERTICAL, 5),
  exitBtn("exit"),
  openBtn("open"), 
  saveBtn("save"), 
  startBtn("start"), 
  stepBtn("step"),
  infoLbl("Info : number of... "),
  nbUpdate(std::to_string(counter)), 
  algLbl("algae: "), 
  corLbl("corals: "),
  scaLbl("scavengers: "),
  nbAlg(std::to_string(sim.getNbAlg())),
  nbCor(std::to_string(sim.getNbCor())),
  nbSca(std::to_string(sim.getNbSca())),
  nbUpdates(std::to_string(counter)),
  generalLbl("General"),
  updateLbl("update: "),
  birthChckBtn("Birth of algae")
{
	set_title("Microrécif");
	set_child(mainBox);
  set_default_size(500,500);
  mainBox.append(interactBox);
  m_Area.setSimulation((simulation)); 
  mainBox.append(m_Area);
  m_Area.set_expand();
	interactBox.append(btnBox);
  interactBox.append(separator);
  interactBox.append(infoBox);
  btnBox.append(generalLbl);
  btnBox.append(exitBtn);
  btnBox.append(openBtn);
  btnBox.append(saveBtn);
  btnBox.append(startBtn);
  btnBox.append(stepBtn);
  btnBox.append(birthChckBtn);
  infoBox.append(infoLbl);
  infoBox.append(updateLbl);
  infoBox.append(nbUpdate);
  infoBox.append(algLbl); infoBox.append(nbAlg);
  infoBox.append(corLbl); infoBox.append(nbCor);
  infoBox.append(scaLbl); infoBox.append(nbSca);
  infoLbl.set_xalign(0.0);
  updateLbl.set_xalign(0.0);
  algLbl.set_xalign(0.0);
  corLbl.set_xalign(0.0);
  scaLbl.set_xalign(0.0); 
  exitBtn.signal_clicked().connect(sigc::mem_fun(*this, &Gui::onButtonExit));
  openBtn.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,
	  &Gui::onButtonSaveOpen), true));
  saveBtn.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,
	  &Gui::onButtonSaveOpen), false));
  startBtn.signal_clicked().connect(sigc::mem_fun(*this,
	    &Gui::onButtonClickedStart));
  stepBtn.signal_clicked().connect(sigc::mem_fun(*this,
	    &Gui::onButtonClickedStep));
  birthChckBtn.signal_toggled().connect(sigc::mem_fun(*this, 
      &Gui::onBirthBox));
  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(
                  sigc::mem_fun(*this, &Gui::onWindowKeyPressed), false);
  add_controller(controller);
}

Gui::~Gui()
{
}


void MyArea::clear()
{
	empty = true; 
	queue_draw();
}

void MyArea::queueDrawArea()
{
  empty = false;
  this->queue_draw();
}

void Gui::onBirthBox() {
  if(spawnAlgae==false) { spawnAlgae = true;} 
  else {spawnAlgae=false;}
}

void Gui::onButtonExit(){
  exit(0);
}


void Gui::onButtonSaveOpen(bool open) 
{ 
  Gtk::FileChooser::Action action = open ?
               Gtk::FileChooser::Action::OPEN : Gtk::FileChooser::Action::SAVE;
  auto dialog = new Gtk::FileChooserDialog("Please choose a file", action);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &Gui::onFileDialogResponse), dialog, open));
  dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  if (open)
  {
    dialog->add_button("_Open", Gtk::ResponseType::OK);
  }
  else
  {
    dialog->add_button("_Save", Gtk::ResponseType::OK);
  }
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("Text files");
	filter_text->add_mime_type("text/plain");
	dialog->add_filter(filter_text);
	auto filter_cpp = Gtk::FileFilter::create();
	filter_cpp->set_name("C/C++ files");
	filter_cpp->add_mime_type("text/x-c");
	filter_cpp->add_mime_type("text/x-c++");
	filter_cpp->add_mime_type("text/x-c-header");
	dialog->add_filter(filter_cpp);
	auto filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog->add_filter(filter_any);
	dialog->show();
}


void Gui::onButtonClickedStart()
{
	if(not timerAdded)
	{	  
    startBtn.set_label("stop");
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		                                        &Gui::onTimeout));
		auto conn = Glib::signal_timeout().connect(my_slot,timeoutValue);
		timerAdded = true;
	}
	else
	{
    startBtn.set_label("start");
		disconnect  = true;   
		timerAdded = false;
	}
}



void Gui::onButtonClickedStep()
{
  counter++;
  sim.update(spawnAlgae); // remember to ADD
  nbUpdate.set_text(std::to_string(counter));
  nbAlg.set_text(to_string(sim.getNbAlg()));
  nbCor.set_text(to_string(sim.getNbCor()));
  nbSca.set_text(to_string(sim.getNbSca()));
  m_Area.queueDrawArea();
}



void Gui::onFileDialogResponse(int response_id, Gtk::FileChooserDialog* dialog, 
                                bool open) {
    switch (response_id) {
        case Gtk::ResponseType::OK: {
            auto filename = dialog->get_file()->get_path();
            if (!open) {
                std::ofstream out_file(filename);
                if (out_file.is_open()) {
                    stringstream dataToWrite;
                    sim.saveSimulation(dataToWrite);
                    string data = dataToWrite.str();
                    out_file << data;
                    out_file.close();
                }
            } else {
                m_Area.clear();
                sim.clearSimulation();
                counter = 0;
                bool open=true;
                sim.readFile(filename.data(),open);
                m_Area.queueDrawArea();
            }
            break;
        }
        case Gtk::ResponseType::CANCEL: {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default: {
            std::cout << "Unexpected button clicked." << std::endl;
            break;
        }
    }
    delete dialog;
}


bool Gui::onTimeout()
{
  if(disconnect)
  {
      disconnect = false;
      return false;
  }
  counter++;
  sim.update(spawnAlgae);

  nbUpdate.set_text(std::to_string(counter));  
  nbAlg.set_text(to_string(sim.getNbAlg()));
  nbCor.set_text(to_string(sim.getNbCor()));
  nbSca.set_text(to_string(sim.getNbSca()));
  if(sim.getNbAlg()==1 and sim.getNbCor()==0 and sim.getNbSca()==0)
  {
  	disconnect  = true;   
  	timerAdded = false;
  }
  m_Area.queueDrawArea();
  return true; 
}


bool Gui::onWindowKeyPressed(guint keyval, guint, Gdk::ModifierType state)
{
	switch(gdk_keyval_to_unicode(keyval))
	{
		case 's':
			onButtonClickedStart();
			return true;
		case '1':
      onButtonClickedStep();
			return true;
	}
    return false;
}