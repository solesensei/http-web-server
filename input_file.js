//Test
var sum = 0,i=0,j=0,cur=1,next=2,temp;

alert("Input Fibonacci number: ");
read(i);
alert("");

while(j<i){
	if(j==0 or j==1){
		alert(1);
	}
	else{
		temp=cur;
		cur=next;
		alert(cur);
		next=temp+next;
	}
	j=j+1;
}
@