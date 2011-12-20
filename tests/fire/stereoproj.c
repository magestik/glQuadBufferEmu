/* This routine performs the perspective projection for one eye's subfield.
   The projection is in the direction of the negative z axis.

   xmin, ymax, ymin, ymax = the coordinate range, in the plane of zero
   parallax setting, that will be displayed on the screen. The ratio between
   (xmax-xmin) and (ymax-ymin) should equal the aspect ration of the display.

   znear, zfar = the z-coordinate values of the clipping planes.

   zzps = the z-coordinate of the plane of zero parallax setting.

   dist = the distance from the center of projection to the plane of zero
   parallax.

   eye = half the eye separation; positive for the right eye subfield,
   negative for the left eye subfield. */
void
stereoproj(float xmin, float xmax, float ymin, float ymax,
  float znear, float zfar, float zzps, float dist, float eye)
{
  float xmid, ymid, clip_near, clip_far, top, bottom, left, right, dx, dy, n_over_d;

  dx = xmax - xmin;
  dy = ymax - ymin;

  xmid = (xmax + xmin) / 2.0;
  ymid = (ymax + ymin) / 2.0;

  clip_near = dist + zzps - znear;
  clip_far = dist + zzps - zfar;

  n_over_d = clip_near / dist;

  top = n_over_d * dy / 2.0;
  bottom = -top;
  right = n_over_d * (dx / 2.0 - eye);
  left = n_over_d * (-dx / 2.0 - eye);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(left, right, bottom, top, clip_near, clip_far);

  glTranslatef(-xmid - eye, -ymid, -zzps - dist);
}

