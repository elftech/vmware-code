{
	print $0|"cat"
	close("cat")
}
{while("who"|getline>0)
	print $0
	close("who")
}
	
	
{
	print $0|"date"
	close("date")
}

