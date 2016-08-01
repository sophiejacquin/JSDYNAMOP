sink('Worst200.txt')
setwd("Test200")
i=10
while(i <=300)
{
	f<-paste ("result_",i, sep = "", collapse = NULL)
	t<-read.table(f)
	print(max(t))
	i<-i+10
}
