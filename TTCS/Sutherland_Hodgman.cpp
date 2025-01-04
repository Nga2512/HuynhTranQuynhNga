#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>


#define MAX_DIEM 20

// Ham tinh giao diem x cua hai doan thang
int tinh_giao_diem_x(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0) {
        return -1; // Tra ve -1 neu doan thang song song, khong co giao diem
    }
    int num = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
    return num / den;
}

// Ham tinh giao diem y cua hai doan thang
int tinh_giao_diem_y(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0) {
        return -1; // Tra ve -1 neu doan thang song song, khong co giao diem
    }
    int num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    return num / den;
}

// Ham cat da giac voi mot canh cua vung xen
void cat_voi_canh(int da_giac[][2], int *so_dinh_da_giac, int x1, int y1, int x2, int y2) {
    int diem_moi[MAX_DIEM][2], so_dinh_moi = 0;

    // Duyet qua cac dinh cua da giac
    for (int i = 0; i < *so_dinh_da_giac; i++) {
        int k = (i + 1) % *so_dinh_da_giac; // Tim dinh ke cua i
        int ix = da_giac[i][0], iy = da_giac[i][1];
        int kx = da_giac[k][0], ky = da_giac[k][1];

        // Tinh vi tri cua hai diem so voi canh clipper
        int i_pos = (x2 - x1) * (iy - y1) - (y2 - y1) * (ix - x1);
        int k_pos = (x2 - x1) * (ky - y1) - (y2 - y1) * (kx - x1);

        // Truong hop 1: Ca hai diem deu ben trong
        if (i_pos < 0 && k_pos < 0) {
            diem_moi[so_dinh_moi][0] = kx;
            diem_moi[so_dinh_moi][1] = ky;
            so_dinh_moi++;
        }
        // Truong hop 2: Chi co diem dau tien ben ngoai
        else if (i_pos >= 0 && k_pos < 0) {
            int giao_x = tinh_giao_diem_x(x1, y1, x2, y2, ix, iy, kx, ky);
            int giao_y = tinh_giao_diem_y(x1, y1, x2, y2, ix, iy, kx, ky);

            if (giao_x != -1 && giao_y != -1) { // Kiem tra giao diem hop le
                diem_moi[so_dinh_moi][0] = giao_x;
                diem_moi[so_dinh_moi][1] = giao_y;
                so_dinh_moi++;
            }

            diem_moi[so_dinh_moi][0] = kx;
            diem_moi[so_dinh_moi][1] = ky;
            so_dinh_moi++;
        }
        // Truong hop 3: Chi co diem thu hai ben ngoai
        else if (i_pos < 0 && k_pos >= 0) {
            int giao_x = tinh_giao_diem_x(x1, y1, x2, y2, ix, iy, kx, ky);
            int giao_y = tinh_giao_diem_y(x1, y1, x2, y2, ix, iy, kx, ky);

            if (giao_x != -1 && giao_y != -1) { // Kiem tra giao diem hop le
                diem_moi[so_dinh_moi][0] = giao_x;
                diem_moi[so_dinh_moi][1] = giao_y;
                so_dinh_moi++;
            }
        }
    }

    *so_dinh_da_giac = so_dinh_moi; // Cap nhat lai so dinh da giac
    for (int i = 0; i < *so_dinh_da_giac; i++) {
        da_giac[i][0] = diem_moi[i][0];
        da_giac[i][1] = diem_moi[i][1];
    }
}

// Ham cat da giac theo vung xen
void cat_da_giac_theo_vung_xen(int da_giac[][2], int *so_dinh_da_giac, int vung_xen[][2], int so_dinh_vung_xen) {
    // Duyet qua moi canh cua vung xen
    for (int i = 0; i < so_dinh_vung_xen; i++) {
        int k = (i + 1) % so_dinh_vung_xen;
        cat_voi_canh(da_giac, so_dinh_da_giac, vung_xen[i][0], vung_xen[i][1], vung_xen[k][0], vung_xen[k][1]);
    }
}

// Ham ve da giac
void ve_da_giac(int da_giac[][2], int so_dinh_da_giac) {
    // Ve tung cap diem lien tiep trong da giac
    for (int i = 0; i < so_dinh_da_giac; i++) {
        int j = (i + 1) % so_dinh_da_giac; // Dinh ke cua dinh i
        line(da_giac[i][0], da_giac[i][1], da_giac[j][0], da_giac[j][1]);
    }
}

// Ham ve vung xen
void ve_vung_xen(int vung_xen[][2], int so_dinh_vung_xen) {
    // Ve tung cap diem lien tiep trong vung xen
    for (int i = 0; i < so_dinh_vung_xen; i++) {
        int j = (i + 1) % so_dinh_vung_xen; // Dinh ke cua dinh i
        line(vung_xen[i][0], vung_xen[i][1], vung_xen[j][0], vung_xen[j][1]);
    }
}

// Ham main
int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, ""); // Khoi tao do hoa

    // Nhap toa do da giac
    int da_giac[MAX_DIEM][2];
    int so_dinh_da_giac;
    printf("Nhap so dinh cua da giac: ");
    scanf("%d", &so_dinh_da_giac);

    // Nhap toa do cac dinh da giac
    printf("Nhap toa do cac dinh da giac:\n");
    for (int i = 0; i < so_dinh_da_giac; i++) {
        printf("Dinh %d: ", i + 1);
        scanf("%d %d", &da_giac[i][0], &da_giac[i][1]);
    }

    // Nhap toa do vung xen
    int vung_xen[MAX_DIEM][2];
    int so_dinh_vung_xen;
    printf("Nhap so dinh cua vung xen: ");
    scanf("%d", &so_dinh_vung_xen);

    // Nhap toa do cac dinh vung xen
    printf("Nhap toa do cac dinh vung xen:\n");
    for (int i = 0; i < so_dinh_vung_xen; i++) {
        printf("Dinh %d: ", i + 1);
        scanf("%d %d", &vung_xen[i][0], &vung_xen[i][1]);
    }

    // Ve da giac ban dau
    setcolor(GREEN);
    ve_da_giac(da_giac, so_dinh_da_giac);

    // Ve vung xen
    setcolor(RED);
    ve_vung_xen(vung_xen, so_dinh_vung_xen);

    // Cat da giac theo vung xen
    cat_da_giac_theo_vung_xen(da_giac, &so_dinh_da_giac, vung_xen, so_dinh_vung_xen);

    // Ve da giac sau khi da bi cat
    setcolor(BLUE);
    ve_da_giac(da_giac, so_dinh_da_giac);

    getch(); // Cho nguoi dung nhan phim
    closegraph(); // Dong do hoa
    return 0;
}

