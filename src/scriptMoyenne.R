sink('Mean20.txt')
setwd("Test20_4")
for(i in 2:300)
{
	f<-paste ("result_",i, sep = "", collapse = NULL)
	t<-read.table(f)
	print(mean(unlist(t)))
}
