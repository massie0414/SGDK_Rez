#ifndef _MESHS_H_
#define _MESHS_H_

#define LINE_NUM 7

extern const Vect3D_f16 cube_coord[10][8];
extern const u16 cube_poly_ind[6 * 4];
extern const u16 cube_line_ind[12 * 2];
extern const Vect3D_f16 cube_face_norm[6];

extern const Vect3D_f16 bg_coord[LINE_NUM * 2];
extern const u16 bg_line_ind[LINE_NUM * 2];

#endif // _MESHS_H_
