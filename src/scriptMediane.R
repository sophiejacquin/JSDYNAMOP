sink('mediane200.txt')
setwd("Test200")
i=10
while(i <=300)
{
	f<-paste ("result_",i, sep = "", collapse = NULL)
	t<-read.table(f)
	k<-unlist(t)
	r<-median(k)
	print(r)
	i<-i+10
}
