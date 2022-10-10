 now=$(date "+%Y-%m-%d")
 echo "Change Directory to C:\Users\Frey\Desktop\ObsdainNote"
 cd C:\Users\Frey\Desktop\ObsdainNote
 echo "Starting add-commit-pull-push..."
 git add -A && git commit -m "$now" && git pull && git push
 echo "Finish!"
