#ifndef CSCOMPONENT_H
#define CSCOMPONENT_H

#include <stdarg.h>

struct iEngine;
struct iLoader;
struct iGraphics3D;
struct iKeyboardDriver;
struct iSector;
struct iView;
struct iVirtualClock;
struct iObjectRegistry;
struct iEvent;
struct iSector;
struct iView;

class CSComponent
{
private:
  iObjectRegistry* object_reg;
  iEngine* engine;
  iLoader* loader;
  iGraphics3D* g3d;
  iKeyboardDriver* kbd;
  iVirtualClock* vc;
  iSector* room;
  iView* view;

public:
  CSComponent ();
  ~CSComponent ();

  // 1st tut
  bool Initialize (int argc, const char* const argv[]);
  void Start ();

  // 2nd tut
  static bool SimpleEventHandler (iEvent& ev);
  bool HandleEvent (iEvent& ev);

  // 4th tut
  void SetupFrame ();
  void FinishFrame ();

};

#endif // CSCOMPONENT_H
