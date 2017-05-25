//Test
var sum = 0;
var i=0;
var j=0;
var cur=1;
var next=2;
var temp;

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