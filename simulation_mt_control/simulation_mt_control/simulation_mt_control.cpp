// simulation_mt_control.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <conio.h>

#define PI 3.1415
#define PI_2 2.0f * PI

//4次ルンゲクッタ法による1次遅れ関数
float rk4(float xin, float xold, float tau, float dt) {
	float k1, k2, k3, k4, xout;

	k1 = dt * (xin - xold) / tau;
	k2 = dt * (xin - (xold + k1 / 2.0f)) / tau;
	k3 = dt * (xin - (xold + k2 / 2.0f)) / tau;
	k4 = dt * (xin - (xold + k3)) / tau;

	xout = xold + (k1 + 2.0f * k2 + 2.0f * k3 + k4) / 6.0f;
	return (xout);
}
//ブラシ付きDCモータの動作シュミレータ
void sim_dcmt(void){
	FILE* fp1;
	float X[9] = { 0.0f };
	float Ra, La, K, J, D, Va, TL, TLs, TLe;
	float ts, te, dt, taue, tauj;
	float t, Tload;

	//init（駆動モータパラメータ)
	Ra = 0.5f;		//電機子抵抗[oum]
	La = 0.008f;	//電機子インダクタンス[H]
	K = 0.28f;		//モータ定数(KE=kT=K)//KE[Vs/rad],Ktau[Nm/A]
	J = 0.005f;		//負荷を含む慣性モーメント[kgm^2]
	D = 0.0002f;	//粘性制動係数[Nms/rad]

	//init（simulation環境設定）
	Va = 3.0f;		//モータ駆動電圧[V]
	TL = 0.0019f;	//負荷トルク[Nm]
	ts = 0.0f;		//simulation開始時間[s]
	te = 1.0f;		//simulation終了時間[s]
	TLs = 0.4f;		//負荷印加開始時間[s]
	TLe = 0.7f;		//負荷印加終了時間[s]

	dt = 0.0005f;	//計算時間間隔[s]
	taue = La / Ra;	//対応時定数
	tauj = J / D;	//同上

	fopen_s(&fp1, "C:\\Users\\fujiurayuuki\\Documents\\vs2019_simulation\\data3.csv", "w");

	for (t = ts; t < te; t += dt) {			//ts~te秒までの繰り返し
		if (t > TLs && t < TLe) {			//TLs<t<TLeのとき
			Tload = TL;						//Tload=TL
		}
		else {								//それ以外，
			Tload = 0.0f;					//Tload=0.0;
		}
		X[0] = Va;							//電機子電圧の設定
		X[1] = X[0] - X[8];					//
		X[2] = X[1] / Ra;					//
		X[3] = rk4(X[2], X[3], taue, dt);	//電機子電流の計算
		X[4] = X[3] * K;					//モータ発生トルクの計算
		X[5] = X[4] - Tload;				//負荷トルクを印加
		X[6] = X[5] / D;					//
		X[7] = rk4(X[6], X[7], tauj, dt);	//回転角速度の計算
		X[8] = X[7] * K;					//
		//時間(sec),回転速度(min-1),電流(A),データファイルの書き込み
		fprintf_s(fp1, "%f,%f,%f\n", t, X[7] * 60.0f / (PI_2), X[3]);
	}
	fclose(fp1);
}

int main()
{
	sim_dcmt();
	return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
