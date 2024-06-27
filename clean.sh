cmd="rm -rf"

clean_build=true
clean_release=true

# Parse command-line arguments
while getopts ":-:" opt; do
  case $opt in
    -)
      case "${OPTARG}" in
        no-build) clean_build=false ;;
        no-release) clean_release=false ;;
        *) exit 1 ;;
      esac;;
    *)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

read -r -p "Are you sure you want to clean? [y/N]: " response
response=${response,,} # tolower
if [[ ! $response =~ ^(y| ) ]]; then
  exit 0
fi

echo "Cleaning..."

do_on_dir () {
  [ -d $1 ] && $cmd $1
}

if $clean_build; then
  do_on_dir "build"
  do_on_dir "build-release/" 
  do_on_dir "build-windows/" 
  do_on_dir "build-windows-release/" 
  do_on_dir "build-web/" 
  do_on_dir "build-web-release/" 
fi
if $clean_release; then
  do_on_dir "release/" 
  do_on_dir "release-web/" 
  do_on_dir "release-windows/" 
fi
