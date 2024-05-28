 now=$(date "+%Y-%m-%d")
 echo "Change Directory to C:\Users\Frey\Desktop\ObsidainNote"
 cd C:\\Users\\Frey\\Desktop\\ObsidainNote
 echo "Starting add-commit-pull-push..."
 git add -A && git commit -m "$now" && git pull && git push
 echo "Finish!"
