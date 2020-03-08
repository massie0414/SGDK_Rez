#ifndef _MESHS_H_
#define _MESHS_H_

#define PLAYER_NUM 8
#define LINE_NUM 7
#define CURSOR_NUM 6
#define CURSOR_A_NUM 4
#define CURSOR_A_ANIM 5
#define ENEMY_NUM 4
#define ENEMY_LOCKON_NUM 4
#define ENEMY_LOCKON_ANIM 5

extern const Vect3D_f16 cube_coord[10][8];
extern const u16 cube_poly_ind[6 * 4];
extern const u16 cube_line_ind[12 * 2];
extern const Vect3D_f16 cube_face_norm[6];

extern const Vect3D_f16 bg_coord[LINE_NUM * 2];
extern const u16 bg_line_ind[LINE_NUM * 2];

extern const Vect3D_f16 cursor_coord[CURSOR_NUM+2];
extern const u16 cursor_line_ind[CURSOR_NUM*2];

extern const Vect3D_f16 cursor_a_coord[CURSOR_A_ANIM][CURSOR_A_NUM];
extern const u16 cursor_a_line_ind[CURSOR_A_NUM*2];

extern const Vect3D_f16 enemy_coord[ENEMY_NUM];
extern const u16 enemy_poly_ind[1*ENEMY_NUM];
extern const Vect3D_f16 enemy_face_norm[1];

extern const Vect3D_f16 enemy_lockon_coord[ENEMY_LOCKON_ANIM][ENEMY_LOCKON_NUM];
extern const u16 enemy_lockon_line_ind[ENEMY_LOCKON_NUM*2];

#endif // _MESHS_H_
