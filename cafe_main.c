#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define SIZE 25
#define INVEN_SIZE 6
//공용체 구분
#define OFFICIAL 1
#define ALBA 2

typedef struct custom {
	char name[SIZE];
	char phone[SIZE];
	int cnt;
}Custom;

typedef struct inven {
	char item[SIZE];
	int cnt;
}Inven;

typedef struct staff {
	int tag;
	char name[SIZE];
	char phone[SIZE];
	union {
		char date[SIZE];
		int month;
	}class;
}Staff;

typedef struct sell{
	char item[SIZE];
	int count;
}Sell;
//-----------------------------------------메뉴 출력-----------------------------------------------
void print_main(){
	printf("\n1. 음료 판매\n");
	printf("2. 고객 관리\n");
	printf("3. 창고 관리\n");
	printf("4. 직원 관리\n");
	printf("5. 마감 처리\n");
}

void print_menu (){
	printf("\n\n-----------Welcome-----------\n");
	printf("1. 아메리카노\t\t2500원\n");
	printf("2. 카페라떼\t\t3000원\n");
	printf("3. 바닐라 라떼\t\t3500원\n");
	printf("4. 카푸치노\t\t3500원\n");
	printf("5. 카라멜 마끼야또\t4000원\n");
	printf("6. 카페모카\t\t4000원\n");
	printf("7. 오늘의 추천 음료!\n");
	printf("0. 나가시려면 0을 눌러주세요\n");
	printf("-----------------------------\n");
	printf("\nSelect : ");
}

void print_membership(){
	printf("---------\n");
	printf("1. 적립\n");
	printf("2. 신규\n");
	printf("---------\n");
	printf("옵션을 선택해 주세요 : ");
}

void print_staff_menu(){
	printf("----------\n");
	printf("1. 신규\n");
	printf("2. 퇴사\n");
	printf("3. 출력\n");
	printf("----------\n");
	printf("옵션을 선택해 주세요 : ");
}
//--------------------------------------------Staff------------------------------------------------
Staff get_srecord(){
	Staff data;
	printf("형식을 선택하세요--> 1. 정직원 2. 알바\n");
	printf("option : ");
	scanf("%d", &data.tag);
	getchar();

	printf("이름 : ");
	scanf("%s", data.name);
	getchar();
	printf("번호 :");
	scanf("%s", data.phone);
	getchar();
	switch (data.tag){
		case OFFICIAL :
			printf("입사일 : ");
			scanf("%s", data.class.date);
			getchar();
			break;
		case ALBA:
			printf("계약 월 수 : ");
			scanf("%d", &data.class.month);
			getchar();
			break;
	}
	return data;
}
void cut_staff(FILE *fsp){
	char name[SIZE];
	Staff data;
	fseek(fsp, 0, SEEK_SET);

	printf("삭제할 직원 이름 : ");
	scanf("%s", name);
	getchar();

	FILE *fstp = fopen("staff_tmp.txt", "a+t");
	if (fstp == NULL){
		printf("open error\n");
		exit(1);
	}
	while (!feof(fsp)){
		fscanf(fsp, "%d", &data.tag);
		if (data.tag == OFFICIAL)
			fscanf(fsp, "%s %s %s\n", data.name, data.phone, data.class.date);
		else if (data.tag ==ALBA)
			fscanf(fsp, "%s %s %d\n", data.name, data.phone, &data.class.month);

		if (strcmp(data.name, name) == 0){
			continue;
		}
		
		if (data.tag == OFFICIAL)
			fprintf(fsp, "%d %s %s %s\n", data.tag, data.name, data.phone, data.class.date);
		else if (data.tag == ALBA)
			fprintf(fsp, "%d %s %s %d\n", data.tag, data.name, data.phone, data.class.month);
	}
	fclose(fstp);
	fclose(fsp);
	remove("staff.txt");
	rename("staff_tmp.txt", "staff.txt");
}

void add_srecord(FILE *fsp){
	Staff data;
	data = get_srecord();
	fseek(fsp, 0, SEEK_END);
	if (data.tag == OFFICIAL)
		fprintf(fsp, "%d %s %s %s\n", data.tag, data.name, data.phone, data.class.date);
	else
		fprintf(fsp, "%d %s %s %d\n", data.tag, data.name, data.phone, data.class.month);
}
void print_srecord(Staff data){
	printf("이름 : %s\n", data.name);
	printf("번호 : %s\n", data.phone);
	if (data.tag == OFFICIAL)
		printf("입사일 : %s\n\n", data.class.date);
	else
		printf("계약 월수 : %d\n\n", data.class.month);
}

void print_staff(FILE *fsp){
	Staff data;
	fseek(fsp, 0, SEEK_SET);
	while(!feof(fsp)){
		fscanf(fsp, "%d", &data.tag);
		if (data.tag == OFFICIAL)
			fscanf(fsp, "%s %s %s\n", data.name, data.phone, data.class.date);
		else if (data.tag == ALBA)
			fscanf(fsp, "%s %s %d\n", data.name, data.phone, &data.class.month);
		else 
			break;
		print_srecord(data);
	}
}

void main_staff(){
	FILE *fsp;	//staff.txt
	int staff_option;
	print_staff_menu();
	if ((fsp = fopen("staff.txt", "a+t")) == NULL){
		fprintf(stderr, "입력을 위한 파일을 열 수 없습니다.");
		exit(1);
	}
	scanf("%d", &staff_option);
	getchar();
	switch (staff_option){
		case 1:
			add_srecord(fsp);	
			fclose(fsp);
			break;
		case 2:
			cut_staff(fsp);
			break;
		case 3:
			print_staff(fsp);
			fclose(fsp);
			break;
		default:
			break;
	}
	printf("\n");
}
//-------------------------------------------판매 관리-----------------------------------------------
int update_total (int *p, int *t, int price){
	*p += price;
	*t += price;
}
//--------------함수 포인터----------------
int check(Inven *invenp, int idx, int cnt){
	invenp[idx].cnt -= cnt;
	return invenp[idx].cnt;
}

void sell_inven(Inven *invenp, int idx, int cnt, int(*func)(Inven *, int, int)){
	if (check(invenp, idx, cnt) < 10){
		invenp[idx].cnt += 50;
		printf("\n<<%s>> 자동 주문 완료\n\n", invenp[idx].item);
	}
}

int idx(Sell *sellp, char *menu){
	int i;
	for (i = 0;i < 6;i++){
		if(strcmp(sellp[i].item, menu)==0)
			break;
	}
	return i;
}

void check_sell(Sell *sellp, char *menu, int(*func)(Sell *sellp, char *)){
	int idx;
	idx = func(sellp, menu);
	if (0> idx|| idx>5)
		printf("\n없는 메뉴입니다.\n");
	else
		printf("%d check\n", idx);
		sellp[idx].count += 1;
}
//------------------------------------------
void init_sell(Sell *sellp){
	char *menu_name[] = {"아메리카노", "카페라떼", "바닐라라떼", "카라멜마끼아또", "카푸치노", "카페모카"};
	for (int i = 0;i < 6;i++){
		strcpy(sellp[i].item, menu_name[i]);
		sellp[i].count = 0;
	}
}

void main_open(int *todayp, Inven *invenp, Sell *sellp){
	int order = 0;
	int flag = 0;
	int total = 0;
	int index;
	init_sell(sellp);
	srand(time(NULL));
	int random;
	while (1){
		if (flag == 1) break;
		print_menu();
		scanf("%d", &order);
		getchar();
		switch (order){
			case 1 :
				check_sell(sellp, "아메리카노", idx);
				update_total(todayp, &total, 2500);
				sell_inven(invenp, 0, 1, check);
				break;
			case 2 :
				check_sell(sellp, "카페라떼", idx);
				update_total(todayp, &total, 3000);
				sell_inven(invenp, 0, 1, check);
				sell_inven(invenp, 2, 2, check);
				break;
			case 3:
				check_sell(sellp, "바닐라라떼", idx);
				update_total(todayp, &total, 3500);
				sell_inven(invenp, 0, 1, check);
				sell_inven(invenp, 2, 1, check);
				sell_inven(invenp, 3, 1, check);
				break;
			case 4:
				check_sell(sellp, "카라멜마끼아또", idx);
				update_total(todayp, &total, 3500);
				sell_inven(invenp, 0, 1, check);
				sell_inven(invenp, 2, 1, check);
				sell_inven(invenp, 4, 1, check);
				break;
			case 5:
				check_sell(sellp, "카푸치노", idx);
				update_total(todayp, &total, 4000);
				sell_inven(invenp, 0, 1, check);
				sell_inven(invenp, 2, 2, check);
				break;
			case 6:
				check_sell(sellp, "카페모카", idx);
				update_total(todayp, &total, 4000);
				sell_inven(invenp, 0, 1, check);
				sell_inven(invenp, 1, 1, check);
				sell_inven(invenp, 2, 1, check);
				sell_inven(invenp, 5, 1, check);
				break;
			case 7:
				random = (rand() % 6) + 1;
				printf("오늘의 추천 음료는 <%d 번!>\n\n", random);
				break;
			case 0:
				flag = 1;
				break;
		}
	}
	printf("\n\n지불해야 될 금액 : %d원\n\n", total);
	//customer 파일 열고
	//메뉴 출력
}
//-----------------------------------------membership-------------------------------------------
Custom get_crecord(){
	Custom data;
	printf("이름 : ");
	scanf("%s", data.name);
	getchar();
	printf("번호 : ");
	scanf("%s", data.phone);
	getchar();
	data.cnt = 1;
	printf("\n");
	return data;
}
//customer 파일 열고 접근하기
void get_stamp(FILE *fcp){
	int flag = 0;
	char phone[SIZE];
	Custom *data = (Custom*)malloc(sizeof(Custom));
	fseek(fcp, 0, SEEK_SET);
	printf("적립할 전화번호를 적어주세요 : ");
	scanf("%s", phone);
	getchar();
	//getchar()
	FILE *fctp = fopen("customer_tmp.dat", "a+t");
	if (fctp == NULL){
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}
	while (!feof(fcp)){
		fread(data, sizeof(Custom), 1, fcp);
		if (strcmp(data->phone, phone)==0){
			data->cnt += 1;
			if (data->cnt == 10){
				printf("해당번호로 쿠폰지급!\n");
				data->cnt = 0;
			}
			printf("쿠폰적립 완료 (현재 개수:%d)\n", data->cnt);
			flag = 1;
			break;
		}
		fwrite(data, sizeof(Custom), 1, fctp);
	}
	if (flag == 0)
		printf("!!잘못된 정보입니다!!.\n\n");
		//다시 메뉴로 돌아가기
	fclose(fctp);
	fclose(fcp);
	free(data);
	
	remove("customer.dat");
	rename("customer_tmp.dat", "customer.dat");
}

void add_member(FILE *fcp){
	Custom *data = (Custom*)malloc(sizeof(Custom));
	*data = get_crecord();
	fseek(fcp, 0, SEEK_END);
	fwrite(data, sizeof(Custom), 1, fcp);
}

void main_membership(){
	FILE *fcp = fopen("customer.dat", "a+t");
	if (fcp == NULL){
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}
	int member_option;
	print_membership();
	scanf("%d", &member_option);
	getchar();
	switch (member_option){
		case 1 :
			get_stamp(fcp);
			break;
		case 2 :
			add_member(fcp);
			fclose(fcp);
			break;
		default :
			break;
	}
}
//------------------------------------------inventory-----------------------------------------
void get_inven(Inven *data){
	FILE *fip = fopen("inventory.txt", "a+t");
	if (fip == NULL){
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}
	for (int i = 0;i < INVEN_SIZE;i++){
		fscanf(fip, "%s %d\n", data[i].item, &data[i].cnt);
	}
	fclose(fip);
}
void print_inven(Inven *data){
	printf("---------창고 현황----------\n");
	for (int i = 0;i < INVEN_SIZE;i++){
		printf("\nNUM : %d\n", i+1);
		printf("Item name : %s\n", data[i].item);
		printf("Count :\t%d\n\n", data[i].cnt);
	}
	printf("----------------------------\n");
}

void update_inven(Inven *data){
	FILE *fip = fopen("inventory.txt", "w+t");
	if (fip==NULL){
		printf("파일을 열 수 없습니다.\n");
		exit(1);
	}
	for (int i = 0;i < INVEN_SIZE;i++){
		fprintf(fip, "%s %d\n", data[i].item, data[i].cnt);
	}
	fclose(fip);
}
//----------------------------------------마감처리---------------------------------------
void print_selling(Sell *sellp){
	printf("----------판매 지표---------\n");
	for(int i = 0;i < 6;i++)
		printf("%8s\t : %3d잔\n", sellp[i].item, sellp[i].count);
	printf("----------------------------\n\n");
}


void main_close(int *tp, Inven *inventory, Sell *sellp) {
	
	printf("\n<마감정산표>\n\n");
	printf("-----------------\n");
	printf("오늘 매출 : %d\n", *tp);
	printf("-----------------\n");

	print_selling(sellp);
	print_inven(inventory);
	
}
	
//---------------------------------------main 함수-----------------------------------------
int main(){
	int m;
	int today_total = 0;
	int *totalp = &today_total;
	//판매지수 관리
	Sell *sellp;
	sellp = (Sell*)malloc(sizeof(Sell) * 6);
	//-------------
	Inven *inventory;//
	inventory = (Inven *)malloc(sizeof(Inven) * INVEN_SIZE);//
	Inven *invenp = inventory;	//
	get_inven(inventory);
	while (1) {
		print_main();
		printf("select option : ");
		scanf("%d", &m);
		getchar();
		if (m == 1){
			main_open(totalp, invenp, sellp);//inventory -> invenp
		}
		else if (m == 2){
			main_membership();
		}
		else if (m == 3){
			print_inven(inventory);
		}
		else if (m == 4){
			main_staff();
		}
		else {
			main_close(totalp, inventory, sellp);
			update_inven(inventory);
			break;
		}
	}
	free(sellp);
	free(inventory);
	return 0;
}
