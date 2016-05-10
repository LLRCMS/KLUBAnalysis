
float Get_SF(int p,float eta, float pt){
	float SF=1.0;
	if (p==1) return 1.0;
	else if (fabs(eta)<0.9){
		if (pt>=25 && pt<30) return 0.9837*0.9995;
		else if (pt>=30 && pt<35) return 0.9841*0.9988;
		else if (pt>=35 && pt<40) return 0.9839*0.9985;
		else if (pt>=40 && pt<50) return 0.9835*0.9985;
		else if (pt>=50 && pt<60) return 0.9843*0.9992;
		else if (pt>=60 && pt<90) return 0.9847*1.0005;
		else if (pt>=90 && pt<140) return 0.9809*1.0007;
		else if (pt>=140) return 0.9804*1.001;
	}
	else if (fabs(eta)>=0.9 && fabs(eta)<1.2){
                if (pt>=25 && pt<30) return 0.9684*1.0022;
                else if (pt>=30 && pt<35) return 0.9654*1.0023;
                else if (pt>=35 && pt<40) return 0.9670*1.0012;
                else if (pt>=40 && pt<50) return 0.9667*1.0002;
                else if (pt>=50 && pt<60) return 0.9627*0.9999;
                else if (pt>=60 && pt<90) return 0.9595*1.0010;
                else if (pt>=90 && pt<140) return 0.9644*1.0012;
		else if (pt>=140) return 0.9713*1.0040;
        }
	else if (fabs(eta)>=1.2){
                if (pt>=25 && pt<30) return 1.005*1.0025;
                else if (pt>=30 && pt<35) return 1.0029*1.001;
                else if (pt>=35 && pt<40) return 0.9962*1.0019;
                else if (pt>=40 && pt<50) return 0.9943*1.000;
                else if (pt>=50 && pt<60) return 0.9905*1.0002;
                else if (pt>=60 && pt<90) return 0.9883*1.0002;
                else if (pt>=90 && pt<140) return 0.9819*0.9999;
		else if (pt>=140) return 0.9973*0.9942;
        }

	else return 1.0;
}

float Get_SF_eff(int p,float eta, float pt){
        float SF=1.0;
	if (p==1) return 1.0;
        else if (fabs(eta)<0.9){
                if (pt>=25 && pt<30) return 0.9995;
                else if (pt>=30 && pt<35) return 0.9988;
                else if (pt>=35 && pt<40) return 0.9985;
                else if (pt>=40 && pt<50) return 0.9985;
                else if (pt>=50 && pt<60) return 0.9992;
                else if (pt>=60 && pt<90) return 1.0005;
                else if (pt>=90 && pt<140) return 1.0007;
                else if (pt>=140) return 1.001;
        }
        else if (fabs(eta)>=0.9 && fabs(eta)<1.2){
                if (pt>=25 && pt<30) return 1.0022;
                else if (pt>=30 && pt<35) return 1.0023;
                else if (pt>=35 && pt<40) return 1.0012;
                else if (pt>=40 && pt<50) return 1.0002;
                else if (pt>=50 && pt<60) return 0.9999;
                else if (pt>=60 && pt<90) return 1.0010;
                else if (pt>=90 && pt<140) return 1.0012;
                else if (pt>=140) return 1.0040;
        }
        else if (fabs(eta)>=1.2){
                if (pt>=25 && pt<30) return 1.0025;
                else if (pt>=30 && pt<35) return 1.0029;
                else if (pt>=35 && pt<40) return 1.0019;
                else if (pt>=40 && pt<50) return 1.000;
                else if (pt>=50 && pt<60) return 1.0002;
                else if (pt>=60 && pt<90) return 1.0002;
                else if (pt>=90 && pt<140) return 0.9999;
                else if (pt>=140) return 0.9973;
        }

        else return 1.0;
}

