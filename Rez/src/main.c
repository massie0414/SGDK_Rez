#include "genesis.h"

#include "meshs.h"

#define MAX_POINTS  256


Vect3D_f16 pts_3D[MAX_POINTS];
Vect2D_s16 pts_2D[MAX_POINTS];


extern Mat3D_f16 MatInv;
extern Mat3D_f16 Mat;


Vect3D_f16 vtab_3D[MAX_POINTS];
Vect2D_s16 vtab_2D[MAX_POINTS];

Rotation3D rotation;
Translation3D translation;
Transformation3D transformation;

Vect3D_f16 rotstep;

fix16 camdist;

u16 flatDrawing;


void updatePointsPos(s16 anim);
void drawPoints(u8 col, u16 x, u16 y);
void doActionJoy(u8 numjoy, u16 value);
void handleJoyEvent(u16 joy, u16 changed, u16 state);


int main()
{
    char str[16];

//    VDP_setScreenWidth256();
    VDP_setScreenWidth320();
//    VDP_setScreenHeight224();
    VDP_setHInterrupt(0);
    VDP_setHilightShadow(0);

    // speed up controller checking
    JOY_setSupport(PORT_1, JOY_SUPPORT_6BTN);
    JOY_setSupport(PORT_2, JOY_SUPPORT_OFF);

    JOY_setEventHandler(handleJoyEvent);

    BMP_init(TRUE, PLAN_A, PAL0, FALSE);

    camdist = FIX16(15);

    M3D_reset();
    M3D_setCamDistance(camdist);
    M3D_setLightEnabled(1);
    M3D_setLightXYZ(FIX16(0.9), FIX16(0.9), FIX16(-0.9));

    // allocate translation and rotation structure
    M3D_setTransform(&transformation, &translation, &rotation);
    M3D_setTranslation(&transformation, FIX16(0), FIX16(0), FIX16(20));
    M3D_setRotation(&transformation, FIX16(0), FIX16(0), FIX16(0));

    flatDrawing = 0;
    s16 anim = 0;

    rotstep.x = FIX16(0.1);
    rotstep.y = FIX16(0.1);

    while (1)
    {
        doActionJoy(JOY_1, JOY_readJoypad(JOY_1));

        M3D_setCamDistance(camdist);

        // do work here
        rotation.x += rotstep.x;
        rotation.y += rotstep.y;
        rotation.z += rotstep.z;
        transformation.rebuildMat = 1;

        anim++;
        anim %= 10;

        updatePointsPos(anim);

        // ensure previous flip buffer request has been started
        BMP_waitWhileFlipRequestPending();
        BMP_showFPS(1);

        BMP_clear();

//        drawPoints(0xFF, -100, -100);
//        drawPoints(0x7F,    0, -100);
//        drawPoints(0x3F,  100, -100);

//        drawPoints(0xFF, -100,    0);
        drawPoints(0x7F,    0,    0);
//        drawPoints(0x3F,  100,    0);

//        drawPoints(0xFF, -100,  100);
//        drawPoints(0x7F,    0,  100);
//        drawPoints(0x3F,  100,  100);

        BMP_drawText("trans z:", 0, 2);
        fix16ToStr(translation.z, str, 2);
        BMP_drawText(str, 10, 2);
        BMP_drawText("cam dist:", 0, 3);
        fix16ToStr(camdist, str, 2);
        BMP_drawText(str, 11, 3);

        BMP_flip(1);
    }
}

void updatePointsPos(s16 anim)
{
	// 指定された3D頂点バッファーへの3D変換（回転および変換）を処理します。
    M3D_transform(
    		&transformation,	// 回転および変換パラメーターを含む変換オブジェクト。
			&cube_coord[anim*8],	// ソース3D頂点バッファー。
			pts_3D,				// 宛先3D頂点バッファー。
			8					// 変換する頂点の数。
	);

    // 指定された3D頂点バッファーへの2D投影を処理します（s16バージョン）。
    M3D_project_s16(
    		pts_3D,	// ソース3D頂点バッファー。
			pts_2D,	// 宛先2D頂点バッファー-s16形式
			8		// 投影する頂点の数。
	);
}

void drawPoints(u8 col, u16 x, u16 y)
{
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

            *pt_dst = pts_2D[*poly_ind++]; pt_dst->x += x; pt_dst->y += y; pt_dst++;
            *pt_dst = pts_2D[*poly_ind++]; pt_dst->x += x; pt_dst->y += y; pt_dst++;
            *pt_dst = pts_2D[*poly_ind++]; pt_dst->x += x; pt_dst->y += y; pt_dst++;
            *pt_dst = pts_2D[*poly_ind++]; pt_dst->x += x; pt_dst->y += y; pt_dst++;

            dp = fix16Mul(transformation.lightInv.x, norm->x) +
                 fix16Mul(transformation.lightInv.y, norm->y) +
                 fix16Mul(transformation.lightInv.z, norm->z);
            norm++;

            if (dp > 0) col += (dp >> (FIX16_FRAC_BITS - 2));

            if (!BMP_isPolygonCulled(v, 4)){
                BMP_drawPolygon(v, 4, col);
            }
        }
//    }
//    else
//    {
    	// ワイヤーフレーム

        Line l;
        const u16 *line_ind;
//        u16 i;

        l.col = col;
        line_ind = cube_line_ind;

        i = 12;

        while (i--)
        {
            l.pt1 = pts_2D[*line_ind++]; l.pt1.x += x; l.pt1.y += y;
            l.pt2 = pts_2D[*line_ind++]; l.pt2.x += x; l.pt2.y += y;

            if (BMP_clipLine(&l)) {
            	BMP_drawLine(&l);
            }
        }
//    }
}


void doActionJoy(u8 numjoy, u16 value)
{
    if (numjoy == JOY_1)
    {
        if (value & BUTTON_UP)
        {
            if (value & BUTTON_A) translation.y += FIX16(0.2);
            else rotstep.x += FIX16(0.05);
        }

        if (value & BUTTON_DOWN)
        {
            if (value & BUTTON_A) translation.y -= FIX16(0.2);
            else rotstep.x -= FIX16(0.05);
        }

        if (value & BUTTON_LEFT)
        {
            if (value & BUTTON_A) translation.x -= FIX16(0.2);
            else rotstep.y += FIX16(0.05);
        }

        if (value & BUTTON_RIGHT)
        {
            if (value & BUTTON_A) translation.x += FIX16(0.2);
            else rotstep.y -= FIX16(0.05);
        }

        if (value & BUTTON_Y)
        {
            if (value & BUTTON_X) camdist += FIX16(1.0);
            else camdist += FIX16(1);
        }

        if (value & BUTTON_Z)
        {
            if (value & BUTTON_X) camdist -= FIX16(1.0);
            else camdist -= FIX16(1);
        }

        if (value & BUTTON_B)
        {
            if (value & BUTTON_A) translation.z += FIX16(1);
            else translation.z += FIX16(1);
        }

        if (value & BUTTON_C)
        {
            if (value & BUTTON_A) translation.z -= FIX16(1);
            else translation.z -= FIX16(1);
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
