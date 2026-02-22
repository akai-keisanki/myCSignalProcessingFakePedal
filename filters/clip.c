float clip(float x, float thr)
{
  if (x > thr) return thr;
  else if (x < -thr) return -thr;
  else return x;
}
