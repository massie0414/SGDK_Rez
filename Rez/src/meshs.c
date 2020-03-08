#include "genesis.h"
#include "meshs.h"

//================================
// プレイヤー
//================================

// 頂点
const Vect3D_f16 cube_coord[10][8] = {
		{
				{FIX16(1.200000),FIX16(1.200000),FIX16(-1.200000)},
				{FIX16(1.200000),FIX16(-1.200000),FIX16(-1.200000)},
				{FIX16(1.200000),FIX16(1.200000),FIX16(1.200000)},
				{FIX16(1.200000),FIX16(-1.200000),FIX16(1.200000)},
				{FIX16(-1.200000),FIX16(1.200000),FIX16(-1.200000)},
				{FIX16(-1.200000),FIX16(-1.200000),FIX16(-1.200000)},
				{FIX16(-1.200000),FIX16(1.200000),FIX16(1.200000)},
				{FIX16(-1.200000),FIX16(-1.200000),FIX16(1.200000)},
		},
		{
				{FIX16(1.159375),FIX16(1.159375),FIX16(-1.159375)},
				{FIX16(1.159375),FIX16(-1.159375),FIX16(-1.159375)},
				{FIX16(1.159375),FIX16(1.159375),FIX16(1.159375)},
				{FIX16(1.159375),FIX16(-1.159375),FIX16(1.159375)},
				{FIX16(-1.159375),FIX16(1.159375),FIX16(-1.159375)},
				{FIX16(-1.159375),FIX16(-1.159375),FIX16(-1.159375)},
				{FIX16(-1.159375),FIX16(1.159375),FIX16(1.159375)},
				{FIX16(-1.159375),FIX16(-1.159375),FIX16(1.159375)},

		},
		{
				{FIX16(1.100000),FIX16(1.100000),FIX16(-1.100000)},
				{FIX16(1.100000),FIX16(-1.100000),FIX16(-1.100000)},
				{FIX16(1.100000),FIX16(1.100000),FIX16(1.100000)},
				{FIX16(1.100000),FIX16(-1.100000),FIX16(1.100000)},
				{FIX16(-1.100000),FIX16(1.100000),FIX16(-1.100000)},
				{FIX16(-1.100000),FIX16(-1.100000),FIX16(-1.100000)},
				{FIX16(-1.100000),FIX16(1.100000),FIX16(1.100000)},
				{FIX16(-1.100000),FIX16(-1.100000),FIX16(1.100000)},
		},
		{
				{FIX16(1.066992),FIX16(1.066992),FIX16(-1.066992)},
				{FIX16(1.066992),FIX16(-1.066992),FIX16(-1.066992)},
				{FIX16(1.066992),FIX16(1.066992),FIX16(1.066992)},
				{FIX16(1.066992),FIX16(-1.066992),FIX16(1.066992)},
				{FIX16(-1.066992),FIX16(1.066992),FIX16(-1.066992)},
				{FIX16(-1.066992),FIX16(-1.066992),FIX16(-1.066992)},
				{FIX16(-1.066992),FIX16(1.066992),FIX16(1.066992)},
				{FIX16(-1.066992),FIX16(-1.066992),FIX16(1.066992)},
		},
		{
				{FIX16(1.042187),FIX16(1.042187),FIX16(-1.042187)},
				{FIX16(1.042187),FIX16(-1.042187),FIX16(-1.042187)},
				{FIX16(1.042187),FIX16(1.042187),FIX16(1.042187)},
				{FIX16(1.042187),FIX16(-1.042187),FIX16(1.042187)},
				{FIX16(-1.042187),FIX16(1.042187),FIX16(-1.042187)},
				{FIX16(-1.042187),FIX16(-1.042187),FIX16(-1.042187)},
				{FIX16(-1.042187),FIX16(1.042187),FIX16(1.042187)},
				{FIX16(-1.042187),FIX16(-1.042187),FIX16(1.042187)},
		},
		{
				{FIX16(1.024414),FIX16(1.024414),FIX16(-1.024414)},
				{FIX16(1.024414),FIX16(-1.024414),FIX16(-1.024414)},
				{FIX16(1.024414),FIX16(1.024414),FIX16(1.024414)},
				{FIX16(1.024414),FIX16(-1.024414),FIX16(1.024414)},
				{FIX16(-1.024414),FIX16(1.024414),FIX16(-1.024414)},
				{FIX16(-1.024414),FIX16(-1.024414),FIX16(-1.024414)},
				{FIX16(-1.024414),FIX16(1.024414),FIX16(1.024414)},
				{FIX16(-1.024414),FIX16(-1.024414),FIX16(1.024414)},
		},
		{
				{FIX16(1.012500),FIX16(1.012500),FIX16(-1.012500)},
				{FIX16(1.012500),FIX16(-1.012500),FIX16(-1.012500)},
				{FIX16(1.012500),FIX16(1.012500),FIX16(1.012500)},
				{FIX16(1.012500),FIX16(-1.012500),FIX16(1.012500)},
				{FIX16(-1.012500),FIX16(1.012500),FIX16(-1.012500)},
				{FIX16(-1.012500),FIX16(-1.012500),FIX16(-1.012500)},
				{FIX16(-1.012500),FIX16(1.012500),FIX16(1.012500)},
				{FIX16(-1.012500),FIX16(-1.012500),FIX16(1.012500)},
		},
		{
				{FIX16(1.005273),FIX16(1.005273),FIX16(-1.005273)},
				{FIX16(1.005273),FIX16(-1.005273),FIX16(-1.005273)},
				{FIX16(1.005273),FIX16(1.005273),FIX16(1.005273)},
				{FIX16(1.005273),FIX16(-1.005273),FIX16(1.005273)},
				{FIX16(-1.005273),FIX16(1.005273),FIX16(-1.005273)},
				{FIX16(-1.005273),FIX16(-1.005273),FIX16(-1.005273)},
				{FIX16(-1.005273),FIX16(1.005273),FIX16(1.005273)},
				{FIX16(-1.005273),FIX16(-1.005273),FIX16(1.005273)},
		},
		{
				{FIX16(1.001563),FIX16(1.001563),FIX16(-1.001563)},
				{FIX16(1.001563),FIX16(-1.001563),FIX16(-1.001563)},
				{FIX16(1.001563),FIX16(1.001563),FIX16(1.001563)},
				{FIX16(1.001563),FIX16(-1.001563),FIX16(1.001563)},
				{FIX16(-1.001563),FIX16(1.001563),FIX16(-1.001563)},
				{FIX16(-1.001563),FIX16(-1.001563),FIX16(-1.001563)},
				{FIX16(-1.001563),FIX16(1.001563),FIX16(1.001563)},
				{FIX16(-1.001563),FIX16(-1.001563),FIX16(1.001563)},
		},
		{
				{FIX16(1.000195),FIX16(1.000195),FIX16(-1.000195)},
				{FIX16(1.000195),FIX16(-1.000195),FIX16(-1.000195)},
				{FIX16(1.000195),FIX16(1.000195),FIX16(1.000195)},
				{FIX16(1.000195),FIX16(-1.000195),FIX16(1.000195)},
				{FIX16(-1.000195),FIX16(1.000195),FIX16(-1.000195)},
				{FIX16(-1.000195),FIX16(-1.000195),FIX16(-1.000195)},
				{FIX16(-1.000195),FIX16(1.000195),FIX16(1.000195)},
				{FIX16(-1.000195),FIX16(-1.000195),FIX16(1.000195)},
		},

};

// 面
const u16 cube_poly_ind[6 * 4] =
{
	0,4,6,2,
	3,2,6,7,
	7,6,4,5,
	5,1,3,7,
	1,0,2,3,
	5,4,0,1,

};

// ワイヤーフレーム
const u16 cube_line_ind[12 * 2] =
{
		0, 4,
		4, 6,
		2, 6,
		6, 7,
		4, 5,
		5, 7,
		1, 3,
		3, 7,
		0, 2,
		2, 3,
		0, 1,
		1, 5,
};

// 法線
const Vect3D_f16 cube_face_norm[6] =
{
    {FIX16( 0), FIX16( 1), FIX16( 0)},
    {FIX16( 0), FIX16( 0), FIX16( 1)},
    {FIX16(-1), FIX16( 0), FIX16( 0)},
    {FIX16( 0), FIX16(-1), FIX16( 0)},
    {FIX16( 1), FIX16( 0), FIX16( 0)},
    {FIX16( 0), FIX16( 0), FIX16(-1)}
};









//================================
// 背景
//================================

const Vect3D_f16 bg_coord[LINE_NUM * 2] = {

		{FIX16(-15),FIX16(-2),FIX16(0)},	//
		{FIX16( 15),FIX16(-2),FIX16(0)},	//

		{FIX16(-15),FIX16(-2),FIX16(20)},	//
		{FIX16( 15),FIX16(-2),FIX16(20)},	//

		{FIX16(-15),FIX16(-2),FIX16(40)},	//
		{FIX16( 15),FIX16(-2),FIX16(40)},	//

		{FIX16(-15),FIX16(-2),FIX16(60)},	//
		{FIX16( 15),FIX16(-2),FIX16(60)},	//

		{FIX16(-15),FIX16(-2),FIX16(80)},	//
		{FIX16( 15),FIX16(-2),FIX16(80)},	//

		{FIX16(-15),FIX16(-2),FIX16(100)},	//
		{FIX16( 15),FIX16(-2),FIX16(100)},	//

		{FIX16(-15),FIX16(-2),FIX16(120)},	//
		{FIX16( 15),FIX16(-2),FIX16(120)},	//


};

// ワイヤーフレーム
const u16 bg_line_ind[LINE_NUM * 2] =
{
		0, 1,
		2, 3,
		4, 5,
		6, 7,
		8, 9,
		10, 11,
		12, 13,
};







//================================
// カーソル
//================================

const Vect3D_f16 cursor_coord[CURSOR_NUM+2] =
{
		{FIX16(-2),FIX16( 2),FIX16(0)},
		{FIX16(-2),FIX16(-2),FIX16(0)},
		{FIX16( 2),FIX16( 2),FIX16(0)},
		{FIX16( 2),FIX16(-2),FIX16(0)},

		{FIX16( 0),FIX16( 1),FIX16(0)},
		{FIX16( 0),FIX16(-1),FIX16(0)},

		{FIX16( 1),FIX16( 0),FIX16(0)},
		{FIX16(-1),FIX16( 0),FIX16(0)},
};

// ワイヤーフレーム
const u16 cursor_line_ind[CURSOR_NUM*2] =
{
		0, 1,
		1, 3,
		3, 2,
		2, 0,

		4, 5,
		6, 7,
};

//================================
// カーソル：Aボタン
//================================

// Aボタンを押したときの反応
const Vect3D_f16 cursor_a_coord[CURSOR_A_ANIM][CURSOR_A_NUM] =
{
		{
			{FIX16(-2.2),FIX16( 2.2),FIX16(0)},
			{FIX16(-2.2),FIX16(-2.2),FIX16(0)},
			{FIX16( 2.2),FIX16( 2.2),FIX16(0)},
			{FIX16( 2.2),FIX16(-2.2),FIX16(0)},
		},
		{
			{FIX16(-2.4),FIX16( 2.4),FIX16(0)},
			{FIX16(-2.4),FIX16(-2.4),FIX16(0)},
			{FIX16( 2.4),FIX16( 2.4),FIX16(0)},
			{FIX16( 2.4),FIX16(-2.4),FIX16(0)},
		},
		{
			{FIX16(-2.6),FIX16( 2.6),FIX16(0)},
			{FIX16(-2.6),FIX16(-2.6),FIX16(0)},
			{FIX16( 2.6),FIX16( 2.6),FIX16(0)},
			{FIX16( 2.6),FIX16(-2.6),FIX16(0)},
		},
		{
			{FIX16(-2.8),FIX16( 2.8),FIX16(0)},
			{FIX16(-2.8),FIX16(-2.8),FIX16(0)},
			{FIX16( 2.8),FIX16( 2.8),FIX16(0)},
			{FIX16( 2.8),FIX16(-2.8),FIX16(0)},
		},
		{
			{FIX16(-3.0),FIX16( 3.0),FIX16(0)},
			{FIX16(-3.0),FIX16(-3.0),FIX16(0)},
			{FIX16( 3.0),FIX16( 3.0),FIX16(0)},
			{FIX16( 3.0),FIX16(-3.0),FIX16(0)},
		},
};

// Aボタンを押したときの反応：ワイヤーフレーム
const u16 cursor_a_line_ind[CURSOR_A_NUM*2] =
{
		0, 1,
		1, 3,
		3, 2,
		2, 0,
};





//================================
// 敵
//================================

const Vect3D_f16 enemy_coord[ENEMY_NUM] =
{
		{FIX16(1.000000),FIX16(1.000000),FIX16(-0.000000)},
		{FIX16(1.000000),FIX16(-1.000000),FIX16(0.000000)},
		{FIX16(-1.000000),FIX16(1.000000),FIX16(0.000000)},
		{FIX16(-1.000000),FIX16(-1.000000),FIX16(0.000000)},
};

// 面
const u16 enemy_poly_ind[1 * ENEMY_NUM] =
{
		0,1,3,2,
		0,1,3,2,
};

// 法線
const Vect3D_f16 enemy_face_norm[1] =
{
		{FIX16( -0.0000),FIX16(-0.0000),FIX16(-1.0000)},
//		{FIX16( -0.0000),FIX16(-0.0000),FIX16(1.0000)},

};

//================================
// 敵：ロックオン
//================================

// 座標
const Vect3D_f16 enemy_lockon_coord[ENEMY_LOCKON_ANIM][ENEMY_LOCKON_NUM] =
{
		{
				{FIX16(5.000000),FIX16(5.000000),FIX16(-0.000001)},
				{FIX16(5.000000),FIX16(-5.000000),FIX16(-0.000001)},
				{FIX16(-5.000000),FIX16(5.000000),FIX16(0.000001)},
				{FIX16(-5.000000),FIX16(-5.000000),FIX16(0.000001)},
		},
		{
				{FIX16(3.813128),FIX16(4.872505),FIX16(0.000004)},
				{FIX16(4.872505),FIX16(-3.813128),FIX16(-0.000005)},
				{FIX16(-4.872505),FIX16(3.813128),FIX16(0.000005)},
				{FIX16(-3.813128),FIX16(-4.872505),FIX16(-0.000004)},
		},
		{
				{FIX16(1.640495),FIX16(3.912643),FIX16(0.000006)},
				{FIX16(3.912643),FIX16(-1.640495),FIX16(-0.000005)},
				{FIX16(-3.912643),FIX16(1.640495),FIX16(0.000005)},
				{FIX16(-1.640495),FIX16(-3.912643),FIX16(-0.000006)},
		},
		{
				{FIX16(0.297920),FIX16(2.278704),FIX16(0.000002)},
				{FIX16(2.278704),FIX16(-0.297920),FIX16(-0.000001)},
				{FIX16(-2.278704),FIX16(0.297920),FIX16(0.000001)},
				{FIX16(-0.297920),FIX16(-2.278704),FIX16(-0.000002)},
		},
		{
				{FIX16(0.012211),FIX16(1.414161),FIX16(-0.000000)},
				{FIX16(1.414161),FIX16(-0.012211),FIX16(0.000000)},
				{FIX16(-1.414161),FIX16(0.012211),FIX16(-0.000000)},
				{FIX16(-0.012211),FIX16(-1.414161),FIX16(0.000000)},

		},
};

// ワイヤーフレーム
const u16 enemy_lockon_line_ind[ENEMY_LOCKON_NUM*2] =
{
		0, 1,
		1, 3,
		3, 2,
		2, 0,
};

