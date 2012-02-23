.text
	.extern  App_Msg
	.global  App_Msg 
	.extern  main
	.global  main

__entry:
		B       main
		B       App_Msg
