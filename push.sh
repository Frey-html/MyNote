 now=$(date "+%Y-%m-%d")
<<<<<<< HEAD
 echo "Change Directory to D:\MyNote"
 cd D:\\MyNote
=======
 echo "Change Directory to C:\Users\Frey\Desktop\ObsidainNote"
 cd C:\\Users\\Frey\\Desktop\\ObsidainNote
>>>>>>> 50920e30de3862eaa2fdaf4972249325fa28b40e
 echo "Starting add-commit-pull-push..."
 git add -A && git commit -m "$now" && git pull && git push
 echo "Finish!"
