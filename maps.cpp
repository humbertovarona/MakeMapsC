//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Maps.h"

USELIB("Surfer.lib");
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
  return 1;
}

extern "C" __declspec(dllexport)
TLimitesParam WINAPI InicMapaParam(int xmin, int ymin, int xmax, int ymax, int ancho, int alto)
{
  limites.XMin = xmin;
  limites.XMax = xmax;
  limites.YMin = ymin;
  limites.YMax = ymax;
  limites.Ancho = ancho;
  limites.Alto = alto;

  return limites;
}
//---------------------------------------------------------------------------

extern "C" __declspec(dllexport)
TPoint WINAPI MundoAMapa(TMundoCoord coord)
{
  int ix, iy;

  ix = limites.Ancho  * (coord.x - limites.XMin) / (limites.XMax - limites.XMin);
  iy = limites.Alto * (coord.y - limites.YMin) / (limites.YMax - limites.YMin);

  return TPoint(ix, iy);
}
//---------------------------------------------------------------------------

extern "C" __declspec(dllexport)
TMundoCoord WINAPI MapaAMundo(TPoint mapcoord)
{
  TMundoCoord MundoCoord;

  MundoCoord.x = mapcoord.x * (limites.XMax - limites.XMin) / limites.Ancho + limites.XMin;
  MundoCoord.y = mapcoord.y * (limites.YMax - limites.YMin) / limites.Alto + limites.YMin;

  return MundoCoord;
}
//---------------------------------------------------------------------------

extern "C" __declspec(dllexport)
int WINAPI ReflexionMapaX(int ix)
{
  ix = limites.Ancho - ix;
  return ix;
}
//---------------------------------------------------------------------------

extern "C" __declspec(dllexport)
int WINAPI ReflexionMapaY(int iy)
{
  iy = limites.Alto - iy;
  return iy;
}
//---------------------------------------------------------------------------

extern "C" __declspec(dllexport)
void WINAPI AbrirMAP(const AnsiString NombredelMapa, TCanvas *Mapa)
{
  char            command, comentario;
  char            texto[256];
  TMundoCoord     a, b;
  TPoint          pa, pb;
  TPenStyle       tipol;
  TBrushStyle     tipob;
  TColor          color, rcolor;
  int             npuntos;
  TPoint          *puntos = NULL;

  FILE *fMapas = fopen(NombredelMapa.c_str(), "rt");

  while (!feof(fMapas))
  {
    command = (char) fgetc(fMapas);
    switch (command)
    {
      case 'c':
      case 'C':
        fscanf(fMapas, "%d%d%d%d%f%f%f%f", &color, &tipol, &rcolor, &tipob, &(a.x), &(a.y), &(b.x), &(b.y));
        pa = MundoAMapa(a);
        pa.y = ReflexionMapaY(pa.y);
        b.y = 0;
        pb = MundoAMapa(b);
        Mapa->Pen->Color = color;
        Mapa->Pen->Style = tipol;
        Mapa->Brush->Color = rcolor;
        Mapa->Brush->Style = tipob;
        Mapa->Ellipse(pa.x, pa.y, pb.x, pb.y);
        break;
      case 'r':
      case 'R':
        fscanf(fMapas, "%d%d%d%d%f%f%f%f", &color, &tipol, &rcolor, &tipob, &(a.x), &(a.y), &(b.x), &(b.y));
        pa = MundoAMapa(a);
        pa.y = ReflexionMapaY(pa.y);
        pb = MundoAMapa(b);
        pb.y = ReflexionMapaY(pb.y);
        Mapa->Pen->Color = color;
        Mapa->Pen->Style = tipol;
        Mapa->Brush->Color = rcolor;
        Mapa->Brush->Style = tipob;
        Mapa->Rectangle(pa.x, pa.y, pb.x, pb.y);
        break;
      case 'l':
      case 'L':
        fscanf(fMapas, "%d%d%f%f%f%f", &color, &tipol, &(a.x), &(a.y), &(b.x), &(b.y));
        pa = MundoAMapa(a);
        pa.y = ReflexionMapaY(pa.y);
        pb = MundoAMapa(b);
        pb.y = ReflexionMapaY(pb.y);
        Mapa->Pen->Color = color;
        Mapa->Pen->Style = tipol;
        Mapa->MoveTo(pa.x, pa.y);
        Mapa->LineTo(pb.x, pb.y);
        break;
      case 't':
      case 'T':
        fscanf(fMapas, "%d%f%f%s", &color, &(a.x), &(a.y), &texto);
        pa = MundoAMapa(a);
        pa.y = ReflexionMapaY(pa.y);
        Mapa->Font->Color = color;
        Mapa->TextOut(pa.x, pa.y, (AnsiString) texto);
        break;
      case 'p':
      case 'P':
        fscanf(fMapas, "%d%d%d", &color, &tipol, &npuntos);
        puntos = new TPoint[npuntos];
        Mapa->Pen->Color = color;
        Mapa->Pen->Style = tipol;
        fscanf(fMapas, "%f%f", &(a.x), &(a.y));
        puntos[0] = MundoAMapa(a);
        puntos[0].y = ReflexionMapaY(puntos[0].y);
        Mapa->MoveTo(puntos[0].x, puntos[0].y);
        for (int i = 1; i < npuntos; i++)
        {
          fscanf(fMapas, "%f%f", &(a.x), &(a.y));
          puntos[i] = MundoAMapa(a);
          puntos[i].y = ReflexionMapaY(puntos[i].y);
          Mapa->LineTo(puntos[i].x, puntos[i].y);
        }
        delete[] puntos;
        break;
      /*
      case 'f':
      case 'F':
        fscanf(fMapas, "%f%f%d", &color, &(a.x), &(a.y));
        pa = MundoAMapa(a);
        pa.y = ReflexionMapaY(pa.y);
        //Mapa->Brush->Color = clRed;
        Mapa->FloodFill(pa.x, pa.y, color, fsSurface);
        break;
        */
      case '#':
        comentario = (char) fgetc(fMapas);
        break;
    }
  }
}

extern "C" __declspec(dllexport)
void WINAPI AbrirPost(const AnsiString Nombre, int var, TCanvas *Mapa)
{
  TVariables Variables;
  TMundoCoord coord;
  TPoint p;
  AnsiString valor;

  CargarVariables(Nombre, 105, Variables);

  for (int i = 0; i < 105; i++)
    if (Variables[i][var] != LIMITE)
    {
      coord.x = Variables[i][coordx];
      coord.y = Variables[i][coordy];
      p = MundoAMapa(coord);
      p.y = ReflexionMapaY(p.y);
      Mapa->Font->Color = clBlue;
      valor = FloatToStr(Variables[i][var]);
      valor.Delete(valor.AnsiPos(".") + 2, valor.Length() - valor.AnsiPos(".") + 2);
      Mapa->TextOut(p.x, p.y, valor);
    }
}

extern "C" __declspec(dllexport)
void WINAPI AbrirVectores(const AnsiString MapaCompX, TRejillaSurfer CompX, const AnsiString MapaCompY, TRejillaSurfer CompY,TCanvas *Mapa)
{
  float modulo;
  TMundoCoord coordp1, coordp2;
  TPoint      p1, p2;

  CargarRejillaSurfer(MapaCompX, CompX);
  CargarRejillaSurfer(MapaCompY, CompY);
  CompX.deltax = (CompX.xmax - CompX.xmin) / (CompX.nx - 1);
  CompX.deltay = (CompX.ymax - CompX.ymin) / (CompX.ny - 1);
  coordp1.y = CompX.ymin;
  for (int j = 0; j < CompX.ny; j++)
  {
    coordp1.x = CompX.xmin;
    for (int i = 0; i < CompX.nx; i++)
    {
      modulo = sqrt(pow(CompX.Rejilla[i][j], 2) + pow(CompX.Rejilla[i][j], 2));
      p1 = MundoAMapa(coordp1);
      p1.y = ReflexionMapaY(p1.y);
      coordp2.x = coordp1.x + CompX.Rejilla[i][j] / modulo * CompX.deltax;
      coordp2.y = coordp1.y + CompY.Rejilla[i][j] / modulo * CompY.deltay;
      p2 = MundoAMapa(coordp2);
      p2.y = ReflexionMapaY(p2.y);
      DibujaFlecha(CalculaFlecha(p1, p2, M_PI/6, 0.25), Mapa);
      coordp1.x += CompX.deltax;
    }
    coordp1.y += CompX.deltay;
  }
}

extern "C" __declspec(dllexport)
void WINAPI AbrirBLN(const AnsiString NombredelMapa, TCanvas *Mapa)
{
  TMundoCoord     a;
  int             npuntos, cerrada;
  TPoint          *puntos = NULL;

  FILE *fMapas = fopen(NombredelMapa.c_str(), "rt");

  while (!feof(fMapas))
  {
    fscanf(fMapas, "%d,%d\n", &npuntos, &cerrada);
    puntos = new TPoint[npuntos];
    fscanf(fMapas, "%f,%f\n", &(a.x), &(a.y));
    puntos[0] = MundoAMapa(a);
    puntos[0].y = ReflexionMapaY(puntos[0].y);
    Mapa->MoveTo(puntos[0].x, puntos[0].y);
    for (int i = 1; i < npuntos; i++)
    {
      fscanf(fMapas, "%f,%f\n", &(a.x), &(a.y));
      puntos[i] = MundoAMapa(a);
      puntos[i].y = ReflexionMapaY(puntos[i].y);
      Mapa->LineTo(puntos[i].x, puntos[i].y);
    }
    delete[] puntos;
  }
}

extern "C" __declspec(dllexport)
TPoint WINAPI rotar(TPoint p, float alpha)
{
  TPoint p1;

  p1.x = (int)(p.x*cos(alpha)-p.y*sin(alpha));
  p1.y = (int)(p.x*sin(alpha)+p.y*cos(alpha));
  return p1;
}

extern "C" __declspec(dllexport)
TPoint WINAPI Trasladar(TPoint p, TPoint p1)
{
  TPoint p2;

  p2.x = p.x + p1.x;
  p2.y = p.y + p1.y;
  return p2;
}


extern "C" __declspec(dllexport)
TFlecha WINAPI CalculaFlecha(TPoint p1, TPoint p2, float alpha, float lzaeta)
{
  float beta1, beta2, gamma;
  TPoint p3, p4;
  TFlecha Flecha;

  Flecha.p1 = p1;
  Flecha.p2 = p2;
  if (lzaeta < 0.25)
    lzaeta = 0.25;
  else
  if (lzaeta > 0.75)
    lzaeta = 0.75;
  float r = lzaeta * sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
  beta1 = M_PI - alpha;
  beta2 = beta1 + 2 * alpha;
  p3.x = r * cos(beta1);
  p3.y = r * sin(beta1);
  p4.x = r * cos(beta2);
  p4.y = r * sin(beta2);
  gamma = atan2(p1.y - p2.y, p1.x - p2.x) + M_PI;
  p3 = rotar(p3, gamma);
  p4 = rotar(p4, gamma);
  p3 = Trasladar(p3, p2);
  p4 = Trasladar(p4, p2);
  Flecha.p3 = p3;
  Flecha.p4 = p4;

  return Flecha;
}

extern "C" __declspec(dllexport)
void WINAPI DibujaFlecha(TFlecha flecha, TCanvas *mapa)
{
  mapa->MoveTo(flecha.p1.x, flecha.p1.y);
  mapa->LineTo(flecha.p2.x, flecha.p2.y);
  mapa->LineTo(flecha.p3.x, flecha.p3.y);
  mapa->MoveTo(flecha.p2.x, flecha.p2.y);
  mapa->LineTo(flecha.p4.x, flecha.p4.y);
}

//---------------------------------------------------------------------------
