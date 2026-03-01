#include "drive.h"

#include "clip.h"

float drive(float x, float thr)
{
  return (clip(x, thr) + x)/2;
}
