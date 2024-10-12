 now=$(date "+%Y-%m-%d")
 echo "Starting add-commit-pull-push..."
 git pull
 git add -A  && git commit -m "$now"  
 git push
 echo "Finish!"
