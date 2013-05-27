-- Name: ordertotal
-- Parameters: onumber = order number
-- 	       taxable = 0 if not taxable, 1 if taxable
--	       ototal  = order total variable

create procedure ordertotal(
	in onumber int,
	in taxable boolean,
	out ototal decimal(8,2)
)comment 'Obtain order total, optionally adding tax'
begin
	declare total decimal(8,2);
	declare taxrate int default 6;
	
	select sum(item_price*quantity)
	from orderitems
	where order_num=onumber
	into total;

	if taxable then 
		select total+(total/100*taxrate) into total;
	end if;
	
	select total into ototal;
end;
