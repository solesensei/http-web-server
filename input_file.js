//Test
function sum(a,b){
	return a+b;
}

function recursion(c){
	if(c>0){
		var x = sum(c,c);
		alert(x);
		recursion(c-1);
	}
	else{
		alert(0);
	}
}

var x;

x=sum(1,3);
alert(x);
x=sum("hi",x);
alert(x);
alert("");
recursion(5);
@