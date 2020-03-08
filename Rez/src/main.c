#include "genesis.h"
#include "meshs.h"
#include "resource.h"

#define MAX_POINTS  256
#define CURSOR_SPEED 2

//extern Mat3D_f16 MatInv;
//extern Mat3D_f16 Mat;

//Vect3D_f16 vtab_3D[MAX_POINTS];
//Vect2D_s16 vtab_2D[MAX_POINTS];

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
		u16 *poly_ind,
		Vect3D_f16 *face_norm,
		u8 col,
		u16 point_num,
		u16 poly_num
);

u16 doActionJoy(
		u8 numjoy,
		u16 value,
		u16 last_key
);

void handleJoyEvent(
		u16 joy,
		u16 changed,
		u16 state
);

s16 anim_a = 0;	// 0の時は非表示

int main()
{
    char str[16];

    SYS_disableInts();

//    VDP_setScreenWidth256();	// メモリが少ないのか、ゴミみたいなのが表示される
    VDP_setScreenWidth320();

    // 水平割り込みを無効にします。
    VDP_setHInterrupt(0);

    // ハイライト/シャドウ効果を無効にします。
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

    Vect3D_f16 pts_3D[PLAYER_NUM];
    Vect2D_s16 pts_2D[PLAYER_NUM];

    Vect3D_f16 pts_bg_3D[LINE_NUM * 2];
    Vect2D_s16 pts_bg_2D[LINE_NUM * 2];

    Vect3D_f16 pts_cursor_3D[CURSOR_NUM+2];
    Vect2D_s16 pts_cursor_2D[CURSOR_NUM+2];

    Vect3D_f16 pts_cursor_a_3D[CURSOR_A_NUM];
    Vect2D_s16 pts_cursor_a_2D[CURSOR_A_NUM];

    Vect3D_f16 pts_enemy_3D[ENEMY_NUM];
    Vect2D_s16 pts_enemy_2D[ENEMY_NUM];

    Vect3D_f16 pts_enemy_lockon_3D[ENEMY_LOCKON_NUM];
    Vect2D_s16 pts_enemy_lockon_2D[ENEMY_LOCKON_NUM];

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

    // カーソル：Aボタンを押したときのエフェクト的な
    Vect3D_f16 cursor_a_coord_tmp[CURSOR_A_ANIM][CURSOR_A_NUM];
    for ( s16 j = 0; j < CURSOR_A_ANIM; j++ ) {
			for ( s16 i = 0; i < CURSOR_A_NUM; i++ ) {
			cursor_a_coord_tmp[j][i].x = cursor_a_coord[j][i].x;
			cursor_a_coord_tmp[j][i].y = cursor_a_coord[j][i].y;
			cursor_a_coord_tmp[j][i].z = cursor_a_coord[j][i].z;
		}
    }

    // 敵
    fix16 enemy_x = FIX16(5);
    fix16 enemy_y = FIX16(5);
    fix16 enemy_z = FIX16(100);

    Vect3D_f16 enemy_coord_tmp[ENEMY_NUM];
    for ( s16 i = 0; i < ENEMY_NUM; i++ ) {
    	enemy_coord_tmp[i].x = enemy_coord[i].x + enemy_x;
    	enemy_coord_tmp[i].y = enemy_coord[i].y + enemy_y;
    	enemy_coord_tmp[i].z = enemy_coord[i].z + enemy_z;
    }

    // 敵：ロックオン
    Vect3D_f16 enemy_lockon_coord_tmp[ENEMY_LOCKON_ANIM][ENEMY_LOCKON_NUM];
    for ( s16 j = 0; j < ENEMY_LOCKON_ANIM; j++ ) {
			for ( s16 i = 0; i < ENEMY_LOCKON_NUM; i++ ) {
			enemy_lockon_coord_tmp[j][i].x = enemy_lockon_coord[j][i].x;
			enemy_lockon_coord_tmp[j][i].y = enemy_lockon_coord[j][i].y;
			enemy_lockon_coord_tmp[j][i].z = enemy_lockon_coord[j][i].z;
		}
    }

    // 色の実験
//    u16 palette_test[16] =
//    {
//        0x0000,
//        0x0222,
//        0x0444,
//        0x0666,
//        0x0888,
//        0x0AAA,
//        0x0CCC,
//        0x0EEE,
//
//        0x0EEE,
//        0x0EEE,
//        0x0EEE,
//        0x0EEE,
//        0x0EEE,
//        0x0EEE,
//        0x0EEE,
//        0x0EEE
//    };
//
//    s16 color = 0x0000;

    // BGMの初期化
//    SND_setVolume_4PCM_ENV(
//            SOUND_PCM_CH1, // @suppress("Symbol is not resolved")
//            15    // Volume to set : 16 possible level from 0 (minimum) to 15 (maximum).
//    );
//    SND_setVolume_4PCM_ENV(
//            SOUND_PCM_CH2, // @suppress("Symbol is not resolved")
//            15    // Volume to set : 16 possible level from 0 (minimum) to 15 (maximum).
//    );
//    SND_setVolume_4PCM_ENV(
//            SOUND_PCM_CH3, // @suppress("Symbol is not resolved")
//            15    // Volume to set : 16 possible level from 0 (minimum) to 15 (maximum).
//    );
//    SND_setVolume_4PCM_ENV(
//            SOUND_PCM_CH4, // @suppress("Symbol is not resolved")
//            15    // Volume to set : 16 possible level from 0 (minimum) to 15 (maximum).
//    );


    // BGMの再生
//    SND_startPlay_XGM(BGM_SAMPLE);

    u16 last_key = 0;

    u8 is_lockon = FALSE;
    s16 anim_lockon = 0;

    SYS_enableInts();

    while (1)
    {
        last_key = doActionJoy(JOY_1, JOY_readJoypad(JOY_1), last_key);

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
			PLAYER_NUM,
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

        if ( anim_a == 1 ) {
        	for ( s16 j = 0; j < CURSOR_A_ANIM; j++ ) {
				for ( s16 i = 0; i < CURSOR_A_NUM; i++ ) {
					cursor_a_coord_tmp[j][i].x = cursor_a_coord[j][i].x + cursor_x;
					cursor_a_coord_tmp[j][i].y = cursor_a_coord[j][i].y + cursor_y;
					cursor_a_coord_tmp[j][i].z = cursor_a_coord[j][i].z + cursor_z;
				}
        	}
        }

        // カーソルの投影:Aボタン
        if ( anim_a >= 1 ) {
			updatePointsPos(
				&cursor_a_coord_tmp,
				pts_cursor_a_3D,
				pts_cursor_a_2D,
				CURSOR_A_NUM,
				anim_a-1
			);
		}

        // 敵：なんちゃって平行移動
        for ( s16 i = 0; i < ENEMY_NUM; i++ ) {
        	enemy_coord_tmp[i].x = enemy_coord[i].x + enemy_x;
        	enemy_coord_tmp[i].y = enemy_coord[i].y + enemy_y;
        	enemy_coord_tmp[i].z = enemy_coord[i].z + enemy_z;
        }
    	// ループ
        enemy_z -= FIX16(1);
    	if ( enemy_z <= FIX16(-20)) {
    		enemy_z += FIX16(150);
    	}

        // 敵：投影
        updatePointsPos(
        	&enemy_coord_tmp,
			pts_enemy_3D,
			pts_enemy_2D,
			ENEMY_NUM,
			0
		);

        // 敵：ロックオン
        if ( anim_a == 1
          && is_lockon == FALSE
          && pts_cursor_2D[0].x <= pts_enemy_2D[0].x  && pts_enemy_2D[0].x <= pts_cursor_2D[0].x + 48
          && pts_cursor_2D[0].y <= pts_enemy_2D[0].y  && pts_enemy_2D[0].y <= pts_cursor_2D[0].y + 48
        ) {
        	is_lockon = TRUE;

        	// 効果音
            SND_setPCM_XGM(65, SE_LOCKON, sizeof(SE_LOCKON));
            SND_startPlayPCM_XGM(65, 10, SOUND_PCM_CH3);
        }

        if ( is_lockon ) {
        	for ( s16 j = 0; j < ENEMY_LOCKON_ANIM; j++ ) {
				for ( s16 i = 0; i < ENEMY_LOCKON_NUM; i++ ) {
					enemy_lockon_coord_tmp[j][i].x = enemy_lockon_coord[j][i].x + enemy_x;
					enemy_lockon_coord_tmp[j][i].y = enemy_lockon_coord[j][i].y + enemy_y;
					enemy_lockon_coord_tmp[j][i].z = enemy_lockon_coord[j][i].z + enemy_z;
				}
        	}
        }

        // 敵：ロックオン
        if ( is_lockon ) {
			updatePointsPos(
				&enemy_lockon_coord_tmp,
				pts_enemy_lockon_3D,
				pts_enemy_lockon_2D,
				ENEMY_LOCKON_NUM,
				anim_lockon
			);
			anim_lockon++;
			if ( anim_lockon >= ENEMY_LOCKON_ANIM ) {
				anim_lockon = ENEMY_LOCKON_ANIM-1;	// 最後のアニメーションにとどめておく
			}
		}

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

	    // カーソルの描画：Aボタン
	    if ( anim_a >= 1 ) {
			l.col = 255;
			line_ind = cursor_a_line_ind;
			for ( s16 i = 0; i < CURSOR_A_NUM; i++ ) {
				l.pt1 = pts_cursor_a_2D[*line_ind++];
				l.pt2 = pts_cursor_a_2D[*line_ind++];
				if (BMP_clipLine(&l)) {
					BMP_drawLine(&l);
				}
			}
			anim_a++;
			if ( anim_a > CURSOR_A_ANIM ) {
				anim_a = 0;
			}
	    }

	    // 敵：描画
	    drawPoints(
	    		pts_enemy_2D,
				enemy_poly_ind,
				enemy_face_norm,
				0x02,
				ENEMY_NUM,
				1
		);

	    // 敵：ロックオン
	    if ( is_lockon ) {
			l.col = 255;
			line_ind = enemy_lockon_line_ind;
			for ( s16 i = 0; i < ENEMY_LOCKON_NUM; i++ ) {
				l.pt1 = pts_enemy_lockon_2D[*line_ind++];
				l.pt2 = pts_enemy_lockon_2D[*line_ind++];
				if (BMP_clipLine(&l)) {
					BMP_drawLine(&l);
				}
			}
	    }

	    // プレイヤーの描画
        drawPoints(
        		pts_2D,
				cube_poly_ind,
				cube_face_norm,
				0x02,
				4,
				6
		);

        // パーティクルテスト（表示はされる）
        /*
        Vect2D_u16 part_pos[1];
        u8 col = 0xFF;
        s16 num = 1;

        part_pos[0].x = 100;
        part_pos[0].y = 100;

        BMP_setPixels_V2D(part_pos, col, num);
        */

        // デバッグ表示
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

//		y++;
//		BMP_drawText("cursor x:", 0, y);
//		fix16ToStr(cursor_x, str, 2);
//		BMP_drawText(str, 10, y);
//
//		y++;
//		BMP_drawText("cursor y:", 0, y);
//		fix16ToStr(cursor_y, str, 2);
//		BMP_drawText(str, 10, y);

//		y++;
//		BMP_drawText("enemy x:", 0, y);
//		fix16ToStr(enemy_x, str, 2);
//		BMP_drawText(str, 10, y);
//
//		y++;
//		BMP_drawText("enemy y:", 0, y);
//		fix16ToStr(enemy_y, str, 2);
//		BMP_drawText(str, 10, y);
//
//		y++;
//		BMP_drawText("enemy z:", 0, y);
//		fix16ToStr(enemy_z, str, 2);
//		BMP_drawText(str, 10, y);

    	// 色変更の実験
//		color++;
//		for ( int i = 0; i < 1; i++ ) {
//			palette_test[i] = color;
//		}
//		VDP_setPaletteColors(0, (u16*) palette_test, 16);
//
//		y++;
//		BMP_drawText("color:", 0, y);
//		intToStr(color, str, 2);
//		BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("last_key:", 0, y);
		intToStr(last_key, str, 2);
		BMP_drawText(str, 10, y);
//
//		y++;
//		BMP_drawText("JOY_1:", 0, y);
//		intToStr(JOY_readJoypad(JOY_1), str, 2);
//		BMP_drawText(str, 10, y);
//
//		y++;
//		BMP_drawText("JOY_1_A:", 0, y);
//		intToStr((JOY_readJoypad(JOY_1) & BUTTON_A), str, 2);
//		BMP_drawText(str, 10, y);
//
		y++;
		BMP_drawText("ANIM_A:", 0, y);
		intToStr(anim_a, str, 2);
		BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("is_lockon:", 0, y);
		intToStr(is_lockon, str, 2);
		BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("cursor.x:", 0, y);
		intToStr(pts_cursor_2D[0].x, str, 2);
		BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("enemy.x:", 0, y);
		intToStr(pts_enemy_2D[0].x, str, 2);
		BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("cursor.y:", 0, y);
		intToStr(pts_cursor_2D[0].y, str, 2);
		BMP_drawText(str, 10, y);

		y++;
		BMP_drawText("enemy.y :", 0, y);
		intToStr(pts_enemy_2D[0].y , str, 2);
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
		u16 *poly_ind,
		Vect3D_f16 *face_norm,
		u8 col,
		u16 point_num,
		u16 poly_num
){
//    if (flatDrawing)
//    {
    	// フラット

        Vect2D_s16 v[point_num];
        const Vect3D_f16 *norm;
        const u16 *tmp_poly_ind;
        u16 i = poly_num;

        norm = face_norm;
        tmp_poly_ind = poly_ind;

        while (i--)
        {
            Vect2D_s16 *pt_dst = v;
            fix16 dp;

            for ( s16 j = 0; j < point_num; j++ ) {
				*pt_dst = pts_2D[*tmp_poly_ind++];
				pt_dst++;
            }

            dp = fix16Mul(transformation.lightInv.x, norm->x) +
                 fix16Mul(transformation.lightInv.y, norm->y) +
                 fix16Mul(transformation.lightInv.z, norm->z);
            norm++;

            if (dp > 0) {
            	col += (dp >> (FIX16_FRAC_BITS - 2));
            }

            if (!BMP_isPolygonCulled(v, point_num)){
                BMP_drawPolygon(v, point_num, col);
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


u16 doActionJoy(
		u8 numjoy,
		u16 value,
		u16 last_key
){
    if (numjoy == JOY_1)
    {
    	if ( ( value & BUTTON_A )
    	   && last_key != BUTTON_A
		) {
    		last_key = BUTTON_A;
			// 効果音
            SND_setPCM_XGM(64, SE_A, sizeof(SE_A));
            SND_startPlayPCM_XGM(64, 10, SOUND_PCM_CH2);
    	}

    	if ( ( value & BUTTON_A ) == 0 ) {
    		// Aを離した
    		last_key = 0;
    	}
    	else {
    		// Aを押している
            // アニメーション開始
    		anim_a = 1;
    	}

        if (value & BUTTON_UP)
        {
			if ( cursor_y <= FIX16(20) ) {
				// カメラの回転
				rotstep.x -= FIX16(0.05);

				// カーソルの移動
				cursor_y += FIX16(CURSOR_SPEED);
			}
        }

        if (value & BUTTON_DOWN)
        {
//            if (value & BUTTON_A) {
//            	translation.y -= FIX16(0.2);
//            }
//            else {
            	if ( cursor_y >= FIX16(-10) ) {
                	// カメラの回転
            		rotstep.x += FIX16(0.05);
            //	}
            //	else {
					// カーソルの移動
					cursor_y -= FIX16(CURSOR_SPEED);
            	}
//            }
        }

        if (value & BUTTON_LEFT)
        {
//            if (value & BUTTON_A) {
//            	translation.x -= FIX16(0.2);
//            }
//            else {
            //	if ( cursor_x >= FIX16(-30) ) {
					// カメラの回転
					rotstep.y -= FIX16(0.05);
            //	}
            //	else {
					// カーソルの移動
					cursor_x -= FIX16(CURSOR_SPEED);
            //	}
//            }
        }

        if (value & BUTTON_RIGHT)
        {
//            if (value & BUTTON_A) {
//            	translation.x += FIX16(0.2);
//            }
//            else {
            	if ( cursor_x <= FIX16(30) ) {
					// カメラの回転
					rotstep.y += FIX16(0.05);
            //	}
            //	else {
					// カーソルの移動
					cursor_x += FIX16(CURSOR_SPEED);
            	}
//            }
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
//            if (value & BUTTON_A) {
//            	translation.z += FIX16(1);
//            }
//            else {
            	translation.z += FIX16(0.1);
//            }
        }

        if (value & BUTTON_C)
        {
//            if (value & BUTTON_A) {
//            	translation.z -= FIX16(1);
//            }
//            else {
            	translation.z -= FIX16(0.1);
//            }
        }

        if (value & BUTTON_START)
        {
            rotstep.x = FIX16(0.0);
            rotstep.y = FIX16(0.0);
        }
    }

    return last_key;
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
