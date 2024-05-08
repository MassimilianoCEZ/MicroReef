/*
 * gui.h
 * Ferrulli Massimiliano
 * Waldorff Carl Johan Traeholt
 * version 12
*/


#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>
#include "graphic_gui.h"
#include "constantes.h"
#include "simulation.h"

static bool spawnAlgae = false;

struct Frame
{
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double asp;
	int height;
	int width;
};

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  virtual ~MyArea();
  void clear();
  void setFrame(Frame x); 
  void adjustFrame(int width, int height);
  void setSimulation(Simulation& simulation);
  void queueDrawArea();
protected:
  void onDraw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
private:
  Frame frame;
  bool empty;
  Simulation* sim;
};





class Gui : public Gtk::Window
{
public:
  Gui(Simulation& simulation);
  virtual ~Gui();
private:
  unsigned int counter;
protected:
  void onFileDialogResponse(int response_id, Gtk::FileChooserDialog* dialog,bool open);
  void onBirthBox();
  void onButtonExit();
  void onButtonSaveOpen(bool open);
  void onButtonClickedStart();
  void onButtonClickedStep();
  bool onTimeout();
  bool onWindowKeyPressed(guint keyval, guint, Gdk::ModifierType state);
  bool timerAdded, disconnect;
  const int timeoutValue;
  Simulation &sim;

  MyArea m_Area;
	Gtk::Box mainBox, interactBox, btnBox, infoBox;
	Gtk::Button exitBtn, openBtn, saveBtn, startBtn, stepBtn;
	Gtk::Label infoLbl, nbUpdate, algLbl, corLbl, scaLbl, nbAlg, nbCor;
  Gtk::Label nbSca, nbUpdates, generalLbl, updateLbl; 
  Gtk::CheckButton birthChckBtn;
  Gtk::Separator separator;
};

#endif //GUI_H
