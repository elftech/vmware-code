account=`cut -d ":" -f1 /etc/passwd|sort`
echo "The following is your linux server's account"
echo "Fist:Unorganized:"
echo $account
echo ""
echo "Second:Organzide:"
for i in $account
do
echo $i
done

