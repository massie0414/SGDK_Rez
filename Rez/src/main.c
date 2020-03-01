#include "genesis.h"
#include "meshs.h"

#define MAX_POINTS  256
#define CURSOR_SPEED 2

extern Mat3D_f16 MatInv;
extern Mat3D_f16 Mat;

Vect3D_f16 vtab_3D[MAX_POINTS];
Vect2D_s16 vtab_2D[MAX_POINTS];

Rotation3D rotation;
Translation3D translation;
Transformation3D transformation;

Vect3D_f16 rotstep;

fix16 camdist;

// フラットかワイヤーフレームか
u16 flatDrawing;

// カーソルの座標
fix16 cursor_x = 0;
fix16 cursor_y = 0;
fix16 cursor_z = FIX16(20);


void updatePointsPos(
		Vect3D_f16 *coord,
		Vect3D_f16 *pts_3D,
		Vect2D_s16 *pts_2D,
		s16 point_num,
		s16 anim
);

void drawPoints(
		Vect2D_s16 *pts_2D,
		u8 col,
		u16 x,
		u16 y
);

void doActionJoy(
		u8 numjoy,
		u16 value
);

void handleJoyEvent(
		u16 joy,
		u16 changed,
		u16 state
);


int main()
{
    char str[16];

//    VDP_setScreenWidth256();
    VDP_setScreenWidth320();

    // 水平割り込みを有効または無効にします。
    VDP_setHInterrupt(0);

    // ハイライト/シャドウ効果を有効または無効にします。
    VDP_setHilightShadow(0);

    // speed up controller checking
    JOY_setSupport(PORT_1, JOY_SUPPORT_6BTN);
    JOY_setSupport(PORT_2, JOY_SUPPORT_OFF);

    JOY_setEventHandler(handleJoyEvent);

    // ソフトウェアビットマップエンジンを初期化します。
    BMP_init(
    		TRUE,	// VRAMダブルバッファを有効にしました。	VRAMダブルバッファにより、部分的な画面更新による画像の破損を回避できます。ほとんどすべてのVRAMタイルスペース（〜41 KB）が必要なので、他のプランやスプライトが必要ない場合にのみ有効にしてください。
			PLAN_A,	//
			PAL0,	//
			FALSE	// ビットマップ計画の優先度を設定します。
	);

    M3D_reset();

    // カメラシーンの距離を設定します。
    camdist = FIX16(15);
    M3D_setCamDistance(
    		camdist
	);

    M3D_setLightEnabled(1);
    M3D_setLightXYZ(FIX16(0.9), FIX16(0.9), FIX16(-0.9));

    // allocate translation and rotation structure
    M3D_setTransform(&transformation, &translation, &rotation);
    M3D_setTranslation(&transformation, FIX16(0), FIX16(-3), FIX16(10));
    M3D_setRotation(&transformation, FIX16(0), FIX16(0), FIX16(0));

    flatDrawing = 1;
    s16 anim = 0;

    // 初期回転速度
    rotstep.x = FIX16(0);
    rotstep.y = FIX16(0);

    Vect3D_f16 pts_3D[MAX_POINTS];
    Vect2D_s16 pts_2D[MAX_POINTS];

    Vect3D_f16 pts_bg_3D[MAX_POINTS];
    Vect2D_s16 pts_bg_2D[MAX_POINTS];

    Vect3D_f16 pts_cursor_3D[MAX_POINTS];
    Vect2D_s16 pts_cursor_2D[MAX_POINTS];

    fix16 player_x = 0;
    fix16 player_y = 0;
    fix16 player_z = 0;

    fix16 line_x = 0;
    fix16 line_y = 0;
    fix16 line_z = 0;

    // 背景：平行移動用に
    Vect3D_f16 bg_coord_tmp[LINE_NUM * 2];
    for ( s16 i = 0; i < LINE_NUM*2; i++ ) {
    	bg_coord_tmp[i].x = bg_coord[i].x;
    	bg_coord_tmp[i].y = bg_coord[i].y;
    	bg_coord_tmp[i].z = bg_coord[i].z;
    }

    // カーソル：平行移動用に
    Vect3D_f16 cursor_coord_tmp[CURSOR_NUM+2];
    for ( s16 i = 0; i < CURSOR_NUM+2; i++ ) {
    	cursor_coord_tmp[i].x = cursor_coord[i].x;
    	cursor_coord_tmp[i].y = cursor_coord[i].y;
    	cursor_coord_tmp[i].z = cursor_coord[i].z;
    }

    while (1)
    {
        doActionJoy(JOY_1, JOY_readJoypad(JOY_1));

        // カメラシーンの距離を設定します。
        M3D_setCamDistance(
        		camdist
		);

        // do work here
        rotation.x = rotstep.x;
        rotation.y = rotstep.y;
        rotation.z = rotstep.z;
        transformation.rebuildMat = 1;	// 回転情報を変更する場合、１に設定する必要がある

        anim++;
        anim %= 10;

        // プレイヤーの投影
        updatePointsPos(
        	&cube_coord,
			pts_3D,
			pts_2D,
			8,
			anim
		);

        // 背景：なんちゃって平行移動
        for ( s16 i = 0; i < LINE_NUM*2; i++ ) {
        //	bg_coord_tmp[i].x = bg_coord[i].x;
        //	bg_coord_tmp[i].y = bg_coord[i].y;
        	bg_coord_tmp[i].z -= FIX16(2);

        	// ループ
        	if ( bg_coord_tmp[i].z <= FIX16(-20)) {
        		bg_coord_tmp[i].z += FIX16(100);
        	}
        }

        // 背景の投影
        updatePointsPos(
        	&bg_coord_tmp,
			pts_bg_3D,
			pts_bg_2D,
			LINE_NUM * 2,
			0
		);

        // カーソル：なんちゃって平行移動
        for ( s16 i = 0; i < CURSOR_NUM+2; i++ ) {
        	cursor_coord_tmp[i].x = cursor_coord[i].x + cursor_x;
        	cursor_coord_tmp[i].y = cursor_coord[i].y + cursor_y;
        	cursor_coord_tmp[i].z = cursor_coord[i].z + cursor_z;
        }

        // カーソルの投影
        updatePointsPos(
        	&cursor_coord_tmp,
			pts_cursor_3D,
			pts_cursor_2D,
			CURSOR_NUM+2,
			0
		);

        // フリップ要求が保留されなくなるまで待ちます。
        BMP_waitWhileFlipRequestPending();

        // FPSの表示
        BMP_showFPS(1);

        // ビットマップバッファをクリアします。
        BMP_clear();


        // 背景の描画
		Line l;
		const u16 *line_ind;

		l.col = 0;
		line_ind = bg_line_ind;

	    for ( s16 i = 0; i < LINE_NUM; i++ ) {

	    	fix16 tmp_z = pts_bg_3D[*line_ind].z;

			l.pt1 = pts_bg_2D[*line_ind++];
			l.pt2 = pts_bg_2D[*line_ind++];

			if ( tmp_z > FIX16( 40 ) ) {
				l.col = 8;	// 半透明っぽい線
			}
			else {
				l.col = 255;	// はっきりした線
			}

			if (BMP_clipLine(&l)) {
				BMP_drawLine(&l);
			}
        }

	    // カーソルの描画
		l.col = 255;
		line_ind = cursor_line_ind;
	    for ( s16 i = 0; i < CURSOR_NUM; i++ ) {
			l.pt1 = pts_cursor_2D[*line_ind++];
			l.pt2 = pts_cursor_2D[*line_ind++];
			if (BMP_clipLine(&l)) {
				BMP_drawLine(&l);
			}
        }



	    // プレイヤーの描画
//        drawPoints(0xFF, -100, -100);
//        drawPoints(0x7F,    0, -100);
//        drawPoints(0x3F,  100, -100);

//        drawPoints(0xFF, -100,    0);
        drawPoints(pts_2D, 0x7F, 0, 0);
//        drawPoints(0x3F,  100,    0);

//        drawPoints(0xFF, -100,  100);
//        drawPoints(0x7F,    0,  100);
//        drawPoints(0x3F,  100,  100);

        int y = 2;
//        BMP_drawText("trans x:", 0, y);
//        fix16ToStr(translation.x, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("trans y:", 0, y);
//        fix16ToStr(translation.y, str, 2);
//        BMP_drawText(str, 10, y);

//        y++;
//        BMP_drawText("trans z:", 0, y);
//        fix16ToStr(translation.z, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("cam dist:", 0, y);
//        fix16ToStr(camdist, str, 2);
//        BMP_drawText(str, 10, y);

//        y++;
//        BMP_drawText("line1 x:", 0, y);
//        intToStr(l.pt1.x, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("line1 y:", 0, y);
//        intToStr(l.pt1.y, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("line2 x:", 0, y);
//        intToStr(l.pt2.x, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("line2 y:", 0, y);
//        intToStr(l.pt2.y, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("cal:", 0, y);
//        intToStr(l.col, str, 2);
//        BMP_drawText(str, 10, y);

//        y++;
//        BMP_drawText("bg[0] z:", 0, y);
//        fix16ToStr(bg_coord_tmp[0].z, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("bg[2] z:", 0, y);
//        fix16ToStr(bg_coord_tmp[2].z, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("bg[4] z:", 0, y);
//        fix16ToStr(bg_coord_tmp[4].z, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("bg[6] z:", 0, y);
//        fix16ToStr(bg_coord_tmp[6].z, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("bg[8] z:", 0, y);
//        fix16ToStr(bg_coord_tmp[8].z, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("bg[10] z:", 0, y);
//        fix16ToStr(bg_coord_tmp[10].z, str, 2);
//        BMP_drawText(str, 10, y);
//
//        y++;
//        BMP_drawText("bg[12] z:", 0, y);
//        fix16ToStr(bg_coord_tmp[12].z, str, 2);
//        BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("cursor x:", 0, y);
		fix16ToStr(cursor_x, str, 2);
		BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("cursor y:", 0, y);
		fix16ToStr(cursor_y, str, 2);
		BMP_drawText(str, 10, y);

        // ビットマップバッファーを画面に切り替えます。
        // 現在のビットマップバックバッファーを画面にブリットしてから、
        // バッファーをフリップし、バックバッファーがフロントバッファーになるようにします。
        // 非同期フラグを使用すると、
        // 空白期間中にビットマップバッファが非同期でビデオメモリに送信され、
        // 関数が即座に戻ります。
        // フリップが既に処理中の場合、
        // フリップリクエストは保留中としてマークされ、
        // 現在のフリップリクエストが完了するとすぐに処理されます。
        // BMP_waitWhileFlipRequestPending（）メソッドを使用して、
        // ビットマップバッファーに書き込む前に保留中のフリップリクエストがないことを
        // 確認できます。
        // フリップリクエストが既に保留中の場合、
        // 機能は保留中のリクエストがなくなるまで待機します。
        BMP_flip(
        	//	0	// 別のフリップがすでに処理中であるため、フリップリクエストが保留中としてマークされている場合は0。
        		1	// フリップリクエストが開始された場合は1。
		);

    }
}

void updatePointsPos(
		Vect3D_f16 *coord,
		Vect3D_f16 *pts_3D,
		Vect2D_s16 *pts_2D,
		s16 point_num,
		s16 anim
) {
	// 指定された3D頂点バッファーへの3D変換（回転および変換）を処理します。
    M3D_transform(
    		&transformation,		// 回転および変換パラメーターを含む変換オブジェクト。
			&coord[anim*point_num],	// ソース3D頂点バッファー。
			pts_3D,				// 宛先3D頂点バッファー。
			point_num				// 変換する頂点の数。
	);

    // 指定された3D頂点バッファーへの2D投影を処理します（s16バージョン）。
    M3D_project_s16(
    		pts_3D,	// ソース3D頂点バッファー。
			pts_2D,		// 宛先2D頂点バッファー-s16形式
			point_num	// 投影する頂点の数。
	);
}

void drawPoints(
		Vect2D_s16 *pts_2D,
		u8 col,
		u16 x,
		u16 y
){
//    if (flatDrawing)
//    {
    	// フラット

        Vect2D_s16 v[4];
        const Vect3D_f16 *norm;
        const u16 *poly_ind;
        u16 i;

        norm = cube_face_norm;
        poly_ind = cube_poly_ind;

        i = 6;

        while (i--)
        {
            Vect2D_s16 *pt_dst = v;
            fix16 dp;
            u8 col = 2;

            *pt_dst = pts_2D[*poly_ind++];
            pt_dst->x += x;
            pt_dst->y += y;
            pt_dst++;

            *pt_dst = pts_2D[*poly_ind++]; pt_dst->x += x; pt_dst->y += y; pt_dst++;
            *pt_dst = pts_2D[*poly_ind++]; pt_dst->x += x; pt_dst->y += y; pt_dst++;
            *pt_dst = pts_2D[*poly_ind++]; pt_dst->x += x; pt_dst->y += y; pt_dst++;

            dp = fix16Mul(transformation.lightInv.x, norm->x) +
                 fix16Mul(transformation.lightInv.y, norm->y) +
                 fix16Mul(transformation.lightInv.z, norm->z);
            norm++;

            if (dp > 0) {
            	col += (dp >> (FIX16_FRAC_BITS - 2));
            }

            if (!BMP_isPolygonCulled(v, 4)){
                BMP_drawPolygon(v, 4, col);
            }
        }
//    }
//    else
//    {
    	// ワイヤーフレーム

//        Line l;
//        const u16 *line_ind;
//        u16 i;
//
//        l.col = col;
//        line_ind = cube_line_ind;
//
//        i = 12;
//
//        while (i--)
//        {
//            l.pt1 = pts_2D[*line_ind++]; l.pt1.x += x; l.pt1.y += y;
//            l.pt2 = pts_2D[*line_ind++]; l.pt2.x += x; l.pt2.y += y;
//
//            if (BMP_clipLine(&l)) {
//            	BMP_drawLine(&l);
//            }
//        }
//    }
}


void doActionJoy(u8 numjoy, u16 value)
{
    if (numjoy == JOY_1)
    {
        if (value & BUTTON_UP)
        {
            if (value & BUTTON_A) {
            	translation.y += FIX16(0.2);
            }
            else {
            	if ( cursor_y <= FIX16(20) ) {
					// カメラの回転
					rotstep.x -= FIX16(0.05);
            //	}
            //	else {
					// カーソルの移動
					cursor_y += FIX16(CURSOR_SPEED);
            	}
            }
        }

        if (value & BUTTON_DOWN)
        {
            if (value & BUTTON_A) {
            	translation.y -= FIX16(0.2);
            }
            else {
            	if ( cursor_y >= FIX16(-10) ) {
                	// カメラの回転
            		rotstep.x += FIX16(0.05);
            //	}
            //	else {
					// カーソルの移動
					cursor_y -= FIX16(CURSOR_SPEED);
            	}
            }
        }

        if (value & BUTTON_LEFT)
        {
            if (value & BUTTON_A) {
            	translation.x -= FIX16(0.2);
            }
            else {
            	if ( cursor_x >= FIX16(-30) ) {
					// カメラの回転
					rotstep.y -= FIX16(0.05);
            //	}
            //	else {
					// カーソルの移動
					cursor_x -= FIX16(CURSOR_SPEED);
            	}
            }
        }

        if (value & BUTTON_RIGHT)
        {
            if (value & BUTTON_A) {
            	translation.x += FIX16(0.2);
            }
            else {
            	if ( cursor_x <= FIX16(30) ) {
					// カメラの回転
					rotstep.y += FIX16(0.05);
            //	}
            //	else {
					// カーソルの移動
					cursor_x += FIX16(CURSOR_SPEED);
            	}
            }
        }

        if (value & BUTTON_Y)
        {
            if (value & BUTTON_X) {
            	camdist += FIX16(1.0);
            }
            else {
            	camdist += FIX16(0.1);
            }
        }

        if (value & BUTTON_Z)
        {
            if (value & BUTTON_X) {
            	camdist -= FIX16(1.0);
            }
            else {
            	camdist -= FIX16(0.1);
            }
        }

        if (value & BUTTON_B)
        {
            if (value & BUTTON_A) {
            	translation.z += FIX16(1);
            }
            else {
            	translation.z += FIX16(0.1);
            }
        }

        if (value & BUTTON_C)
        {
            if (value & BUTTON_A) {
            	translation.z -= FIX16(1);
            }
            else {
            	translation.z -= FIX16(0.1);
            }
        }

        if (value & BUTTON_START)
        {
            rotstep.x = FIX16(0.0);
            rotstep.y = FIX16(0.0);
        }
    }
}

void handleJoyEvent(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1)
    {
        if (changed & ~state & BUTTON_START)
        {
            flatDrawing = 1 - flatDrawing;
        }
    }
}
