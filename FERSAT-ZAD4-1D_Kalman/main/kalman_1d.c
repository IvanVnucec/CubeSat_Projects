#include "kalman_1d.h"


void K1D_Init(Kalman1D_Handle_t *hk1d, float x, float P) {
	hk1d->x = x;
	hk1d->P = P;
}


void K1D_Predict(Kalman1D_Handle_t *hk1d, float dx, float Q) {
	hk1d->x = hk1d->x + dx;
	hk1d->P = hk1d->P + Q;
}


void K1D_Update(Kalman1D_Handle_t *hk1d, float z, float R) {
	float residual, K;

	residual = z - hk1d->x;
	K = hk1d->P / (hk1d->P + R);
	hk1d->x = hk1d->x + K * residual;
	hk1d->P = (1.0f - K) * hk1d->P;
}


float K1D_Output(const Kalman1D_Handle_t *hk1d) {
	return hk1d->x;
}
