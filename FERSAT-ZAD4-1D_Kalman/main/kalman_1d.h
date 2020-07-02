#ifndef INC_1D_KALMAN_H_
#define INC_1D_KALMAN_H_


#ifdef __cplusplus
extern "C" {
#endif


typedef struct Kalman1D_Handle_s {
	float x;
	float P;
} Kalman1D_Handle_t;


void  K1D_Init	 (Kalman1D_Handle_t *hk1d, float x, float P);
void  K1D_Predict(Kalman1D_Handle_t *hk1d, float dx, float Q);
void  K1D_Update (Kalman1D_Handle_t *hk1d, float z, float R);
float K1D_Output(const Kalman1D_Handle_t *hk1d);


#ifdef __cplusplus
}
#endif


#endif /* INC_1D_KALMAN_H_ */
