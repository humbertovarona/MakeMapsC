//---------------------------------------------------------------------------
#ifndef MapH
#define MapH

#include <windows.h>
#include <graphics.hpp>
#include <stdio.h>
#include <math.h>
#include "surfer.h"

typedef struct {
  int XMin, XMax;
  int YMin, YMax;
  int Ancho, Alto;
} TLimitesParam;

typedef struct
{
  float x;
  float y;
} TMundoCoord;

struct TFlecha {
  TPoint p1, p2, p3, p4;
};

TLimitesParam limites;

extern "C" __declspec(dllexport)
TLimitesParam WINAPI InicMapaParam(int xmin, int ymin, int xmax, int ymax, int ancho, int alto);

extern "C" __declspec(dllexport)
TPoint WINAPI MundoAMapa(TMundoCoord coord);

extern "C" __declspec(dllexport)
TMundoCoord WINAPI MapaAMundo(TPoint mapcoord);

extern "C" __declspec(dllexport)
int WINAPI ReflexionMapaX(int ix);

extern "C" __declspec(dllexport)
int WINAPI ReflexionMapaY(int iy);

extern "C" __declspec(dllexport)
TPoint WINAPI rotar(TPoint p, float alpha);

extern "C" __declspec(dllexport)
TPoint WINAPI Trasladar(TPoint p, TPoint p1);

extern "C" __declspec(dllexport)
void WINAPI AbrirPost(const AnsiString Nombre, int var, TCanvas *Mapa);

extern "C" __declspec(dllexport)
void WINAPI AbrirVectores(const AnsiString MapaCompX, TRejillaSurfer CompX, const AnsiString MapaCompY, TRejillaSurfer CompY,TCanvas *Mapa);

extern "C" __declspec(dllexport)
void WINAPI AbrirMAP(const AnsiString NombredelMapa, TCanvas *Mapa);

extern "C" __declspec(dllexport)
void WINAPI AbrirBLN(const AnsiString NombredelMapa, TCanvas *Mapa);

extern "C" __declspec(dllexport)
TFlecha WINAPI CalculaFlecha(TPoint p1, TPoint p2, float alpha, float lzaeta);

extern "C" __declspec(dllexport)
void WINAPI DibujaFlecha(TFlecha flecha, TCanvas *mapa);
//---------------------------------------------------------------------------
#endif
