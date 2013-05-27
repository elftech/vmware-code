#include "stdio.h"

char *num_words[10]={
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

void printOnesTens(int num)
{
	static char *ten_words[10]={
		"","","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
	};
	static char *special_tens[10]={
		"ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen",
		"eighteen","nineteen"
	};
	if(num>99||num<0)
		printf("ERROR: Parameter Out of Range\n");
	int ten_num=num/10;
	int one_num=num%10;
	if(1==ten_num){
		printf("%s ",special_tens[one_num]);
		return;
	}
	if(ten_num!=1&&ten_num!=0)
		printf("%s ",ten_words[ten_num]);
	printf("%s ",num_words[one_num]);

}
void floatToStrings(double num,int precision=10)
{
	static char *big_words[5]={
		"",
		"thousand",
		"million",
		"billion",
		"trillion",
	};

		long long tmp,out_digit_num,out_digit_count;
		tmp=1;
		out_digit_count=1;
		while(tmp<num){
			++out_digit_count;
			tmp*=1000;
		}
		if(tmp!=1){
			tmp/=1000;
			--out_digit_count;
		}
		if(out_digit_count>5){
			printf("ERROR: Float num is too big\n");
			return;
		}

		bool bFirst=true;
		while(out_digit_count>0){
			int hundred_num,ten_one_nums,tmp_num;
			tmp_num=(long long )num/tmp;
			hundred_num=tmp_num/100;
			ten_one_nums=tmp_num%100;
			if(hundred_num!=0)
				printf("%s hundred ",num_words[hundred_num]);
			if(ten_one_nums!=0||bFirst)
				printOnesTens(ten_one_nums);
			bFirst=false;

			--out_digit_count;
			num-=(hundred_num*100+ten_one_nums)*tmp;
			tmp/=1000;
			if(0!=out_digit_count)
				printf("%s ",big_words[out_digit_count]);
		}

		if(0!=num)
			printf("point ");
		while(0!=num&&precision>0){
			num*=10;
			printf("%s ",num_words[(int)num]);
			num-=(float)((int)num);
			--precision;
		}
		printf("\n");
}

int main(int argc, char* argv[])
{
	
	floatToStrings(0);
	floatToStrings(12);
	floatToStrings(0.12343434);
	floatToStrings(1234.56);
	floatToStrings(123432);
	floatToStrings(1234567890123456.1321421);
	floatToStrings(34567890123456.1321421);
	return 0;
}

