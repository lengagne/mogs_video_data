_MoGS_video_tracking()
{
    local argc cur opts
    COMPREPLY=()

    argc=${COMP_CWORD}
    cur="${COMP_WORDS[argc]}"
   
    args=("${COMP_WORDS[@]}")
    unset args[${#args[@]}-1]

    opts=("$(MoGS_video_tracking  "-completion" ${args[*]:1} args)")
    reply=($opts)

    case ${reply[0]} in
	file:)
		_filedir "@(${reply[1]})"
		;;
	folder:)
		_filedir "@()"
		;;
	*)
		COMPREPLY=( $(compgen -W "$opts" -- $cur ) )
		;;
    esac	

}
complete -F _MoGS_video_tracking MoGS_video_tracking
