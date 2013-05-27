create procedure processorders()
begin
	declare done boolean default 0;
	declare o int;
	declare t decimal(8,2);

	declare ordernumbers cursor
	for
	select order_num from orders;

	declare continue handler for sqlstate '02000' set done =1;

	create table if not exists ordertotals
		(order_num int,total decimal(8,2));

	open ordernumbers;

	repeat
		fetch ordernumbers into o;

		call ordertotal(o,1,t);

		insert into ordertotals(order_num,total)
		values(o,t);
		until done end repeat;

	close ordernumbers;
end;
