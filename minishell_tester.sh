#!/bin/bash

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Variables globales
MINISHELL_PATH="./minishell"
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
TIMEOUT_DURATION=5
VERBOSE=0

# Fonction pour afficher l'aide
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help          Afficher cette aide"
    echo "  -m, --minishell     Chemin vers votre minishell (défaut: ./minishell)"
    echo "  -t, --timeout       Timeout en secondes pour chaque test (défaut: 5)"
    echo "  -v, --verbose       Mode verbeux (affiche tous les détails)"
    echo ""
    echo "Exemple:"
    echo "  $0 -m /path/to/minishell -t 10 -v"
}

# Fonction pour parser les arguments
parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -m|--minishell)
                MINISHELL_PATH="$2"
                shift 2
                ;;
            -t|--timeout)
                TIMEOUT_DURATION="$2"
                shift 2
                ;;
            -v|--verbose)
                VERBOSE=1
                shift
                ;;
            *)
                echo "Option inconnue: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# Fonction CORRIGÉE pour exécuter une commande avec timeout
run_command() {
    local cmd="$1"
    local shell="$2"
    local temp_script=$(mktemp)
    local temp_stdout=$(mktemp)
    local temp_stderr=$(mktemp)
    local temp_exitcode=$(mktemp)
    local temp_wrapper=$(mktemp)

    # Créer le script de commande
    echo "$cmd" > "$temp_script"
    echo "exit" >> "$temp_script"

    # Créer un wrapper qui capture le vrai exit code
    if [[ "$shell" == "bash" ]]; then
        cat > "$temp_wrapper" << 'EOF'
#!/bin/bash
bash --posix < "$1" > "$2" 2> "$3"
echo $? > "$4"
EOF
    else
        cat > "$temp_wrapper" << EOF
#!/bin/bash
"$MINISHELL_PATH" < "\$1" > "\$2" 2> "\$3"
echo \$? > "\$4"
EOF
    fi

    chmod +x "$temp_wrapper"

    local timeout_exit_code
    local exit_code
    local error_msg=""

    # Exécuter avec timeout
    timeout "$TIMEOUT_DURATION" "$temp_wrapper" "$temp_script" "$temp_stdout" "$temp_stderr" "$temp_exitcode"
    timeout_exit_code=$?

    if [[ $timeout_exit_code -eq 124 ]]; then
        # Timeout
        exit_code=124
        error_msg="TIMEOUT"
    else
        # Lire le vrai exit code depuis le fichier dédié
        if [[ -f "$temp_exitcode" && -s "$temp_exitcode" ]]; then
            exit_code=$(cat "$temp_exitcode" 2>/dev/null | tr -d '[:space:]')
            [[ -z "$exit_code" || ! "$exit_code" =~ ^[0-9]+$ ]] && exit_code=1
        else
            exit_code=1
        fi

        # Lire stderr en nettoyant les caractères de contrôle
        if [[ -f "$temp_stderr" ]]; then
            error_msg=$(cat "$temp_stderr" 2>/dev/null | tr -d '\0' | sed 's/[[:cntrl:]]//g' | head -10 | tr '\n' ' ' | sed 's/[[:space:]]*$//')
        fi
    fi

    # Nettoyage
    rm -f "$temp_script" "$temp_stdout" "$temp_stderr" "$temp_wrapper" "$temp_exitcode"

    # Utiliser un séparateur plus sûr
    echo "EXITCODE:${exit_code}:ENDEXITCODE:${error_msg}:ENDERROR"
}

# Fonction pour formater l'affichage de la commande
format_command_display() {
    local cmd="$1"
    if [[ -z "$cmd" ]]; then
        echo "[entrée vide]"
    elif [[ "$cmd" =~ ^[[:space:]]+$ ]]; then
        # Remplacer les espaces par des points visibles
        echo "\"$(echo "$cmd" | sed 's/ /·/g' | sed 's/\t/→/g')\""
    else
        echo "$cmd"
    fi
}

# Fonction pour extraire l'exit code et l'erreur de façon sûre
extract_result() {
    local result="$1"
    local exit_code=$(echo "$result" | sed -n 's/.*EXITCODE:\([^:]*\):ENDEXITCODE.*/\1/p')
    local error_msg=$(echo "$result" | sed -n 's/.*:ENDEXITCODE:\(.*\):ENDERROR.*/\1/p')
    
    # Nettoyer l'exit code
    exit_code=$(echo "$exit_code" | tr -d '[:space:]')
    [[ -z "$exit_code" || ! "$exit_code" =~ ^[0-9]+$ ]] && exit_code=1
    
    echo "${exit_code}|${error_msg}"
}

# Fonction pour exécuter un test de comparaison
run_comparison_test() {
    local cmd="$1"
    local display_cmd=$(format_command_display "$cmd")
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Exécuter avec minishell
    local mini_result=$(run_command "$cmd" "mini")
    local mini_extracted=$(extract_result "$mini_result")
    local mini_exit=$(echo "$mini_extracted" | cut -d'|' -f1)
    local mini_error=$(echo "$mini_extracted" | cut -d'|' -f2-)

    # Exécuter avec bash
    local bash_result=$(run_command "$cmd" "bash")
    local bash_extracted=$(extract_result "$bash_result")
    local bash_exit=$(echo "$bash_extracted" | cut -d'|' -f1)
    local bash_error=$(echo "$bash_extracted" | cut -d'|' -f2-)
    
    # Normaliser les messages d'erreur pour la comparaison
    local mini_error_normalized=$(echo "$mini_error" | sed 's/minishell:/bash:/g' | tr -d '[:space:]')
    local bash_error_normalized=$(echo "$bash_error" | tr -d '[:space:]')
    
    # Comparer les résultats
    local test_status=""
    if [[ "$mini_exit" == "$bash_exit" ]]; then
        if [[ -z "$mini_error" && -z "$bash_error" ]] || [[ "$mini_error_normalized" == "$bash_error_normalized" ]]; then
            # Test parfait
            test_status="✅"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            # Exit code correct mais message d'erreur différent
            test_status="✅⚠️"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        fi
    else
        # Exit code différent = échec
        test_status="❌"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    
    # Afficher le résultat
    echo -e "Test $TOTAL_TESTS: $test_status $display_cmd"
    
    # Afficher les détails en cas d'échec ou de mode verbeux
    if [[ "$test_status" == "❌" ]]; then
        echo "  mini exit code = $mini_exit"
        echo "  bash exit code = $bash_exit"
        echo "  mini error = ($mini_error)"
        echo "  bash error = ($bash_error)"
    elif [[ "$test_status" == "✅⚠️" ]]; then
        echo "  mini error = ($mini_error)"
        echo "  bash error = ($bash_error)"
    elif [[ $VERBOSE -eq 1 ]]; then
        echo "  mini exit code = $mini_exit"
        echo "  bash exit code = $bash_exit" 
        echo "  mini error = ($mini_error)"
        echo "  bash error = ($bash_error)"
    fi
}
# Fonction principale de test
run_all_tests() {
    echo -e "${BLUE}=== TESTEUR MINISHELL vs BASH ===${NC}"
    echo -e "${BLUE}Minishell: $MINISHELL_PATH${NC}"
    echo -e "${BLUE}Timeout: $TIMEOUT_DURATION secondes${NC}"
    echo ""
    
    # Vérifier que le minishell existe
    if [[ ! -f "$MINISHELL_PATH" ]]; then
        echo -e "${RED}Erreur: Minishell non trouvé à $MINISHELL_PATH${NC}"
        exit 1
    fi
    
    # Tests de base - Entrées vides et espaces
    echo -e "${YELLOW}=== Tests de base ===${NC}"
    run_comparison_test ""
    run_comparison_test "   "
    run_comparison_test $'\t\t\t'
    
    # Tests de caractères spéciaux
    echo -e "${YELLOW}=== Tests de caractères spéciaux ===${NC}"
    run_comparison_test ":"
    run_comparison_test "!"
    run_comparison_test ">"
    run_comparison_test "<"
    run_comparison_test ">>"
    run_comparison_test "<<"
    run_comparison_test "<>"
    run_comparison_test ">>>>>"
    run_comparison_test ">>>>>>>>>>>>>>>"
    run_comparison_test "<<<<<"
    run_comparison_test "<<<<<<<<<<<<<<<<"
    run_comparison_test "> > > >"
    run_comparison_test ">> >> >> >>"
    run_comparison_test ">>>> >> >> >>"
    
    # Tests de répertoires et chemins
    echo -e "${YELLOW}=== Tests de répertoires et chemins ===${NC}"
    run_comparison_test "/"
    run_comparison_test "//"
    run_comparison_test "/."
    run_comparison_test "/./../../../../.."
    run_comparison_test "///////"
    run_comparison_test "-"
    
    # Tests de pipes et opérateurs
    echo -e "${YELLOW}=== Tests de pipes et opérateurs ===${NC}"
    run_comparison_test "|"
    run_comparison_test "| hola"
    run_comparison_test "| | |"
    run_comparison_test "||"
    run_comparison_test "||||"
    run_comparison_test "||||||||||||"
    run_comparison_test ">>|><"
    run_comparison_test "&&"
    run_comparison_test "&&&&&"
    run_comparison_test "&&&&&&&&&&&&&&"
    run_comparison_test ";;"
    run_comparison_test ";;;;;"
    run_comparison_test ";;;;;;;;;;;;;;;"
    run_comparison_test "()"
    run_comparison_test "( ( ) )"
    run_comparison_test "( ( ( ( ) ) ) )"
    
    # Tests de quotes et caractères spéciaux
    echo -e "${YELLOW}=== Tests de quotes et caractères spéciaux ===${NC}"
    run_comparison_test '""'
    run_comparison_test '"hola"'
    run_comparison_test "'hola'"
    run_comparison_test "''"
    run_comparison_test "*"
    run_comparison_test "."
    run_comparison_test ".."
    run_comparison_test "~"
    run_comparison_test "ABC=hola"
    run_comparison_test "4ABC=hola"
    
    # Tests de commandes inexistantes
    echo -e "${YELLOW}=== Tests de commandes inexistantes ===${NC}"
    run_comparison_test "hola"
    run_comparison_test "hola que tal"
    run_comparison_test "Makefile"
    
    # Tests echo de base
    echo -e "${YELLOW}=== Tests echo de base ===${NC}"
    run_comparison_test "echo"
    run_comparison_test "echo -n"
    run_comparison_test "echo Hola"
    run_comparison_test "echoHola"
    run_comparison_test "echo-nHola"
    run_comparison_test "echo -n Hola"
    run_comparison_test 'echo "-n" Hola'
    run_comparison_test "echo -nHola"
    run_comparison_test "echo Hola -n"
    run_comparison_test "echo Hola Que Tal"
    run_comparison_test "echo         Hola"
    run_comparison_test "echo    Hola     Que    Tal"
    
    # Tests echo avec options
    echo -e "${YELLOW}=== Tests echo avec options ===${NC}"
    run_comparison_test "echo -n -n"
    run_comparison_test "echo -n -n Hola Que"
    run_comparison_test "echo -p"
    run_comparison_test "echo -nnnnn"
    run_comparison_test "echo -n -nnn -nnnn"
    run_comparison_test "echo -n-nnn -nnnn"
    run_comparison_test "echo -n -nnn hola -nnnn"
    run_comparison_test "echo -n -nnn-nnnn"
    run_comparison_test "echo --------n"
    run_comparison_test "echo -nnn --------n"
    run_comparison_test "echo -nnn -----nn---nnnn"
    run_comparison_test "echo -nnn --------nnnn"
    
    # Tests echo avec variables
    echo -e "${YELLOW}=== Tests echo avec variables ===${NC}"
    run_comparison_test "echo \$"
    run_comparison_test "echo \$?"
    run_comparison_test "echo \$?\$"
    run_comparison_test "echo my shit terminal is [\$TERM]"
    run_comparison_test "echo my shit terminal is [\$TERM4"
    run_comparison_test "echo my shit terminal is [\$TERM4]"
    run_comparison_test "echo \$UID"
    run_comparison_test "echo \$HOME9"
    run_comparison_test "echo \$9HOME"
    run_comparison_test "echo \$HOME%"
    run_comparison_test "echo \$UID\$HOME"
    run_comparison_test "echo Le path de mon HOME est \$HOME"
    run_comparison_test "echo \$hola*"
    run_comparison_test "echo -nnnn \$hola"
    run_comparison_test "echo \$DONTEXIST"
    
    # Tests echo avec redirections et pipes (erreurs de syntaxe)
    echo -e "${YELLOW}=== Tests echo avec syntaxe invalide ===${NC}"
    run_comparison_test "echo > <"
    run_comparison_test "echo | |"
    run_comparison_test "EechoE"
    run_comparison_test ".echo."
    run_comparison_test ">echo>"
    run_comparison_test "<echo<"
    run_comparison_test ">>echo>>"
    run_comparison_test "|echo|"
    run_comparison_test "|echo -n hola"

    # Tests echo avec quotes complexes
    echo -e "${YELLOW}=== Tests echo avec quotes complexes ===${NC}"
    run_comparison_test 'echo $""'
    run_comparison_test 'echo "$"""'
    run_comparison_test "echo '\$'''"
	run_comparison_test 'echo "'"'"'\$'"'"''"'"'"'
	run_comparison_test 'echo $"$"'
	run_comparison_test "echo '\$''"
    run_comparison_test 'echo $"HOME"'
    run_comparison_test "echo \$''HOME"
    run_comparison_test 'echo $""HOME'
    run_comparison_test 'echo "$HO"ME'
    run_comparison_test "echo '\$HO'ME"
    run_comparison_test 'echo "$HO""ME"'
    run_comparison_test "echo '\$HO''ME'"
    run_comparison_test 'echo "'"'"'$HO'"'"''"'"'ME'"'"'"'
    run_comparison_test 'echo ""$HOME'
    run_comparison_test 'echo "" $HOME'
    run_comparison_test "echo ''\$HOME"
    run_comparison_test "echo '' \$HOME"
    run_comparison_test 'echo $"HO""ME"'
    run_comparison_test "echo \$'HO''ME'"
    run_comparison_test "echo \$'HOME'"
    run_comparison_test 'echo "$"HOME'
    run_comparison_test "echo \$=HOME"
    run_comparison_test 'echo $"HOLA"'
    run_comparison_test "echo \$'HOLA'"
    run_comparison_test "echo \$DONTEXIST Hola"
    
    # Tests echo avec quotes basiques
    echo -e "${YELLOW}=== Tests echo avec quotes basiques ===${NC}"
    run_comparison_test 'echo "hola"'
    run_comparison_test "echo 'hola'"
    run_comparison_test "echo ''hola''"
    run_comparison_test "echo ''h'o'la''"
    run_comparison_test 'echo "'"'"''"'"'h'"'"'o'"'"'la'"'"''"'"'"'
    run_comparison_test 'echo "'"'"'h'"'"'o'"'"'la"'"'"'"'
    run_comparison_test "echo\"'hola'\""
    run_comparison_test "echo \"'hola'\""
    run_comparison_test 'echo '"'"'\"hola\"'"'"''
    run_comparison_test "echo '''ho\"''''l\"a'''"
    run_comparison_test 'echo hola""""""""""""'
    run_comparison_test "echo hola\"''''''''''\""
    run_comparison_test "echo hola''''''''''''"
    run_comparison_test "echo hola'\"\"\"\"\"\"\"\"\"\"'"
    run_comparison_test 'e"cho hola"'
    run_comparison_test "e'cho hola'"
    run_comparison_test 'echo ""hola'
    run_comparison_test 'echo "" hola'
    run_comparison_test 'echo ""             hola'
    run_comparison_test 'echo hola""bonjour'
    run_comparison_test '"e"'"'"'c'"'"'ho '"'"'b'"'"'"o"nj"o"'"'"'u'"'"'r'
    run_comparison_test '""e"'"'"'c'"'"'ho '"'"'b'"'"'"o"nj"o"'"'"'u'"'"'r"'
    run_comparison_test 'echo "$DONTEXIST"Makefile'
    run_comparison_test 'echo "$DONTEXIST""Makefile"'
    run_comparison_test 'echo "$DONTEXIST" "Makefile"'
    
    # Tests avec variables comme commandes
    echo -e "${YELLOW}=== Tests avec variables comme commandes ===${NC}"
    run_comparison_test "\$?"
    run_comparison_test "\$?\$?"
    run_comparison_test "?\$HOME"
    run_comparison_test "\$"
    run_comparison_test "\$HOME"
    run_comparison_test "\$HOMEdskjhfkdshfsd"
    run_comparison_test '"\$HOMEdskjhfkdshfsd"'
    run_comparison_test "'\$HOMEdskjhfkdshfsd'"
    run_comparison_test "\$DONTEXIST"
    run_comparison_test "\$LESS\$VAR"
    
    # Tests export
    echo -e "${YELLOW}=== Tests export ===${NC}"
    run_comparison_test "export HOLA=bonjour"
    run_comparison_test "export       HOLA=bonjour"
    run_comparison_test "export"
    run_comparison_test "export Hola"
    run_comparison_test "export Hola9hey"
    run_comparison_test "export \$DONTEXIST"
    run_comparison_test 'export ""'
    run_comparison_test "export ="
    run_comparison_test "export %"
    run_comparison_test "export \$?"
    run_comparison_test "export ?=2"
    run_comparison_test "export 9HOLA="
    run_comparison_test "export HOLA9=bonjour"
    run_comparison_test "export _HOLA=bonjour"
    run_comparison_test "export ___HOLA=bonjour"
    run_comparison_test "export _HO_LA_=bonjour"
    run_comparison_test "export HOL@=bonjour"
    run_comparison_test "export HOLA-=bonjour"
    run_comparison_test "export HO-LA=bonjour"
    run_comparison_test "export HOL.A=bonjour"
    run_comparison_test "export HOL}A=bonjour"
    run_comparison_test "export HOL{A=bonjour"
    run_comparison_test "export HO*LA=bonjour"
    run_comparison_test "export HO#LA=bonjour"
    run_comparison_test "export HO@LA=bonjour"
    run_comparison_test "export HO!LA=bonjour"
    run_comparison_test "export HO\$?LA=bonjour"
    run_comparison_test "export +HOLA=bonjour"
    run_comparison_test "export HOL+A=bonjour"
    run_comparison_test "export HOLA+=bonjour"
    run_comparison_test "exportHOLA=bonjour"
    run_comparison_test "export HOLA =bonjour"
    run_comparison_test "export HOLA = bonjour"
    run_comparison_test "export HOLA=bon jour"
    run_comparison_test "export HOLA= bonjour"
    run_comparison_test "export HOLA=\$HOME"
    run_comparison_test "export HOLA=bonjour\$HOME"
    run_comparison_test "export HOLA=\$HOMEbonjour"
    run_comparison_test "export HOLA=bon\$jour"
    run_comparison_test "export HOLA=bon(jour"
    run_comparison_test "export HOLA=bon()jour"
    run_comparison_test "export HOLA=bon@jour"
    run_comparison_test "export HOLA=bon!jour"
    run_comparison_test 'export HOLA=bon""jour""'
    run_comparison_test "export HOLA\$USER=bonjour"
    run_comparison_test "export HOLA=bonjour=casse-toi"
    run_comparison_test 'export ""HOLA=bonjour""=casse-toi'
    run_comparison_test "export HOLA=bonjour BYE=casse-toi"
    run_comparison_test "export \$HOLA=bonjour"
    run_comparison_test 'export HOLA=""bonjour      ""'
    run_comparison_test 'export HOLA=""   -n bonjour   ""'
    run_comparison_test 'export HOLA=""bonjour   ""/'
    run_comparison_test 'export HOLA='"'"'""'"'"''
    run_comparison_test 'export """" HOLA=bonjour'
    run_comparison_test 'export HOLA=""cat Makefile | grep NAME""'
    run_comparison_test 'export HOLA=""  bonjour  hey  ""'
    run_comparison_test 'export TE+S=T'
    run_comparison_test 'export ""=""'
    run_comparison_test "export ''=''"
    run_comparison_test 'export "="="="'
    run_comparison_test "export '='='='"
    run_comparison_test 'export HOL=A=""'
    run_comparison_test "export HOL=A=''"
    
    # Tests unset
    echo -e "${YELLOW}=== Tests unset ===${NC}"
    run_comparison_test "unset INEXISTANT"
    run_comparison_test 'unset ""'
    run_comparison_test "unset PATH"
    run_comparison_test "unset PWD"
    run_comparison_test "unset OLDPWD"
    run_comparison_test "unset 9HOLA"
    run_comparison_test "unset HOLA9"
    run_comparison_test "unset HOL?A"
    run_comparison_test "unset HOLA HOL?A"
    run_comparison_test "unset HOL?A HOLA"
    run_comparison_test "unset HOL?A HOL.A"
    run_comparison_test "unset HOLA="
    run_comparison_test "unset HOL.A"
    run_comparison_test "unset HOL+A"
    run_comparison_test "unset HOL=A"
    run_comparison_test "unset HOL{A"
    run_comparison_test "unset HOL}A"
    run_comparison_test "unset HOL-A"
    run_comparison_test "unset _HOLA"
    run_comparison_test "unset HOL_A"
    run_comparison_test "unset HOLA_"
    run_comparison_test "unset HOL*A"
    run_comparison_test "unset HOL#A"
    run_comparison_test "unset \$HOLA"
    run_comparison_test "unset \$PWD"
    run_comparison_test "unset HOL@"
    run_comparison_test "unset HOL!A"
    run_comparison_test "unset HOL^A"
    run_comparison_test "unset HOL\$?A"
    run_comparison_test 'unset """" HOLA'
    run_comparison_test "unset ="
    run_comparison_test "unset ======"
    run_comparison_test "unset ++++++"
    run_comparison_test "unset _______"
    run_comparison_test "unset export"
    run_comparison_test "unset echo"
    run_comparison_test "unset pwd"
    run_comparison_test "unset cd"
    run_comparison_test "unset unset"
    run_comparison_test "unset sudo"
    
    # Tests pwd
    echo -e "${YELLOW}=== Tests pwd ===${NC}"
    run_comparison_test "pwd"
    run_comparison_test "pwd hola"
    run_comparison_test "pwd ./hola"
    run_comparison_test "pwd hola que tal"
    run_comparison_test "pwd -- p"
    run_comparison_test "pwd pwd pwd"
    run_comparison_test "pwd ls"
    run_comparison_test "pwd ls env"
    
    # Tests cd
    echo -e "${YELLOW}=== Tests cd ===${NC}"
    run_comparison_test "cd"
    run_comparison_test "cd ."
    run_comparison_test "cd ./"
    run_comparison_test "cd ./././."
    run_comparison_test "cd ././././"
    run_comparison_test "cd .."
    run_comparison_test "cd ../"
    run_comparison_test "cd ../.."
    run_comparison_test "cd ../."
    run_comparison_test "cd .././././."
    run_comparison_test "cd srcs objs"
    run_comparison_test "cd sr"
    run_comparison_test "cd Makefile"
    run_comparison_test "cd ../../../../../../.."
    run_comparison_test "cd .././../.././../bin/ls"
    run_comparison_test "cd _"
    run_comparison_test "cd minishell Docs crashtest.c"
    run_comparison_test "cd ~"
    run_comparison_test "cd ~/"
    
    # Tests exit
    echo -e "${YELLOW}=== Tests exit ===${NC}"
    run_comparison_test "exit"
    run_comparison_test "exit exit"
    run_comparison_test "exit hola"
    run_comparison_test "exit hola que tal"
    run_comparison_test "exit 42"
    run_comparison_test "exit 000042"
    run_comparison_test "exit 666"
    run_comparison_test "exit 666 666"
    run_comparison_test "exit -666 666"
    run_comparison_test "exit hola 666"
    run_comparison_test "exit 666 666 666 666"
    run_comparison_test "exit 666 hola 666"
    run_comparison_test "exit hola 666 666"
    run_comparison_test "exit 259"
    run_comparison_test "exit -4"
    run_comparison_test "exit -42"
    run_comparison_test "exit -0000042"
    run_comparison_test "exit -259"
    run_comparison_test "exit -666"
    run_comparison_test "exit +666"
    run_comparison_test "exit 0"
    run_comparison_test "exit +0"
    run_comparison_test "exit -0"
    run_comparison_test "exit +42"
    run_comparison_test "exit -69 -96"
    run_comparison_test "exit --666"
    run_comparison_test "exit ++++666"
    run_comparison_test "exit ++++++0"
    
    # Tests de pipes simples
    echo -e "${YELLOW}=== Tests de pipes simples ===${NC}"
    run_comparison_test "echo hola | cat"
    run_comparison_test "ls | grep test"
    run_comparison_test "echo | echo"
    run_comparison_test "echo \$? | echo \$? | echo \$?"
    
    # Tests de redirections
    echo -e "${YELLOW}=== Tests de redirections ===${NC}"
    run_comparison_test "echo hola > /tmp/test_mini"
    run_comparison_test "echo hola >> /tmp/test_mini"
    
    # Tests avec binaires système
    echo -e "${YELLOW}=== Tests avec binaires système ===${NC}"
    run_comparison_test "/bin/echo"
    run_comparison_test "/bin/echo Hola Que Tal"
    run_comparison_test "/bin/env"
    run_comparison_test "/bin/cd Desktop"
    
    # Tests avec pipes et opérateurs complexes
    echo -e "${YELLOW}=== Tests avec pipes et opérateurs complexes ===${NC}"
    run_comparison_test "export hola | unset hola | echo \$?"
    run_comparison_test "env|\"wc\" -l"
    run_comparison_test "env|\"wc \"-l"
    run_comparison_test "expr 1 + 1"
    run_comparison_test "expr \$? + \$?"
    
    # Tests avec caractères d'échappement et quotes complexes
    echo -e "${YELLOW}=== Tests avec échappements complexes ===${NC}"
    run_comparison_test "echo \"         \" | cat -e"
    run_comparison_test "echo           | cat -e"
    run_comparison_test "\"\"''echo hola\"\"'''' que\"\"'' tal\"\"''"
    run_comparison_test "echo \$:\$= | cat -e"
    run_comparison_test "echo \" \$ \" | cat -e"
    run_comparison_test "echo ' \$ ' | cat -e"
    run_comparison_test "echo \"hola     \" | cat -e"
    
    # Tests avec variables complexes dans quotes
    echo -e "${YELLOW}=== Tests avec variables dans quotes ===${NC}"
    run_comparison_test "echo A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8"
    run_comparison_test "echo \"\$A'\$B\"\"'\$C\"\"\$D'\$E'\"\"'\$F'\"\"'\$G'\$H\"\"\""
    
    # Tests avec permissions
    echo -e "${YELLOW}=== Tests avec permissions ===${NC}"
    run_comparison_test "ls hola"
    run_comparison_test "./Makefile"
    
    # Tests avec cd et variables
    echo -e "${YELLOW}=== Tests cd avec variables ===${NC}"
    run_comparison_test "cd \$HOME"
    run_comparison_test "cd \$HOME \$HOME"
    run_comparison_test "cd \$HOME/42_works"
    run_comparison_test "cd \"\$PWD/srcs\""
    run_comparison_test "cd '\$PWD/srcs'"
    
    # Tests avec cd et quotes
    echo -e "${YELLOW}=== Tests cd avec quotes ===${NC}"
    run_comparison_test "cd 'srcs'"
    run_comparison_test "cd \"srcs\""
    run_comparison_test "cd '/etc'"
    run_comparison_test "cd /e'tc'"
    run_comparison_test "cd /e\"tc\""
    run_comparison_test "cd '/'"
    
    # Tests exit avec différents arguments
    echo -e "${YELLOW}=== Tests exit avec arguments ===${NC}"
    run_comparison_test 'exit "666"'
    run_comparison_test "exit '666'"
    run_comparison_test "exit '-666'"
    run_comparison_test "exit '+666'"
    run_comparison_test "exit '----666'"
    run_comparison_test "exit '++++666'"
    run_comparison_test "exit '6'66"
    run_comparison_test "exit '2'66'32'"
    run_comparison_test "exit \"'666'\""
    run_comparison_test "exit '\"666\"'"
    run_comparison_test "exit '666'\"666\"666"
    run_comparison_test "exit +'666'\"666\"666"
    run_comparison_test "exit -'666'\"666\"666"
    run_comparison_test "exit 9223372036854775807"
    run_comparison_test "exit 9223372036854775808"
    run_comparison_test "exit -9223372036854775808"
    run_comparison_test "exit -9223372036854775809"
    
    # Tests de pipes avec exit
    echo -e "${YELLOW}=== Tests pipes avec exit ===${NC}"
    run_comparison_test "ls | exit"
    run_comparison_test "ls | exit 42"
    run_comparison_test "exit | ls"
    
    # Tests de pipes basiques
    echo -e "${YELLOW}=== Tests pipes ===${NC}"
    run_comparison_test "echo | echo"
    run_comparison_test "echo hola | echo que tal"
    run_comparison_test "pwd | echo hola"
    run_comparison_test "env | echo hola"
    run_comparison_test "env | dontexist"
    run_comparison_test "echo oui | cat -e"
    run_comparison_test "echo oui | echo non | echo hola | grep oui"
    run_comparison_test "echo oui | echo non | echo hola | grep non"
    run_comparison_test "echo oui | echo non | echo hola | grep hola"
    run_comparison_test "echo hola | cat -e | cat -e | cat -e"
    
    # Tests de pipes avec commandes inexistantes
    echo -e "${YELLOW}=== Tests pipes avec erreurs ===${NC}"
    run_comparison_test "ls | hola"
    run_comparison_test "ls | ls hola"
    run_comparison_test "ls | ls | hola"
    run_comparison_test "ls | hola | ls"
    run_comparison_test "ls | ls | hola | rev"
    run_comparison_test "ls | ls | echo hola | rev"
    
    # Tests de pipes avec espaces
    echo -e "${YELLOW}=== Tests pipes avec espaces ===${NC}"
    run_comparison_test "echo hola | cat"
    run_comparison_test "echo hola| cat"
    run_comparison_test "echo hola |cat"
    run_comparison_test "echo hola|cat"
    
    # # Tests de parenthèses
    # echo -e "${YELLOW}=== Tests parenthèses ===${NC}"
    # run_comparison_test "(ls)"
    # run_comparison_test "( ( ls ) )"
    # run_comparison_test "( ( ) ls )"
    # run_comparison_test "ls && (ls)"
    # run_comparison_test "(ls && pwd)"
    # run_comparison_test "( ( ls&&pwd ) )"
    # run_comparison_test "( ( ls ) &&pwd )"
    # run_comparison_test "(ls && ( ( pwd ) ) )"
    # run_comparison_test "( ( ( ( ( pwd) ) ) ) )"
    # run_comparison_test "() pwd"
    # run_comparison_test "(ls||pwd)&&(ls||pwd)"
    # run_comparison_test "(lss||pwd)&&(lss||pwd)"
    # run_comparison_test "(lss&&pwd)&&(lss&&pwd)"
    
    # Tests de redirections simples
    echo -e "${YELLOW}=== Tests redirections de base ===${NC}"
    run_comparison_test "echo hola > bonjour"
    run_comparison_test "echo que tal >> bonjour"
    run_comparison_test "cat < bonjour"
    run_comparison_test "pwd>bonjour"
    run_comparison_test "pwd >                     bonjour"
    
    # Tests d'erreurs de redirections
    echo -e "${YELLOW}=== Tests erreurs redirections ===${NC}"
    run_comparison_test "echo hola > > bonjour"
    run_comparison_test "echo hola < < bonjour"
    run_comparison_test "echo hola >>> bonjour"
    run_comparison_test "> bonjour echo hola"
    run_comparison_test "> bonjour | echo hola"
    
    # Tests de redirections multiples
    echo -e "${YELLOW}=== Tests redirections multiples ===${NC}"
    run_comparison_test "echo hola > hello >> hello >> hello"
    run_comparison_test "echo hola >> hello >> hello > hello"
    run_comparison_test "echo hola > bonjour > hello > bonjour"
    run_comparison_test "echo hola > bonjour >> hello > bonjour"
    run_comparison_test "echo hola > bonjour > hello >> bonjour"
    run_comparison_test "echo hola >> bonjour > hello > bonjour"
    run_comparison_test "echo hola >> bonjour >> hello >> bonjour"
    
    # Tests avec fichiers inexistants
    echo -e "${YELLOW}=== Tests fichiers inexistants ===${NC}"
    run_comparison_test "< pwd"
    run_comparison_test "cat <pwd"
    run_comparison_test "cat <srcs/pwd"
    run_comparison_test "cat <../pwd"
    run_comparison_test "cat < ls"
    run_comparison_test "cat < ls > ls"
    
    # Tests de redirections avec erreurs
    echo -e "${YELLOW}=== Tests redirections avec erreurs ===${NC}"
    run_comparison_test "cat >>"
    run_comparison_test "cat >>>"
    run_comparison_test "cat >> <<"
    run_comparison_test "cat >> > >> << >>"
    
    # Tests avec espaces dans les redirections
    echo -e "${YELLOW}=== Tests redirections avec espaces ===${NC}"
    run_comparison_test "echo hola >bonjour"
    run_comparison_test "cat <bonjour"
    run_comparison_test "echo hola>bonjour"
    run_comparison_test "cat<bonjour"
    run_comparison_test "echo hola> bonjour"
    run_comparison_test "cat< bonjour"
    run_comparison_test "echo hola               >bonjour"
    run_comparison_test "cat<                     bonjour"
    run_comparison_test "echo hola          >     bonjour"
    run_comparison_test "cat            <         bonjour"
    
    # Tests heredoc
    echo -e "${YELLOW}=== Tests heredoc ===${NC}"
    run_comparison_test 'cat << hola
$HOME
hola'
    run_comparison_test "cat << 'hola'
\$HOME
hola"
    run_comparison_test 'cat << "hola"
$HOME
hola'
    run_comparison_test 'cat << ho"la"
$HOME
hola'
    run_comparison_test '<< hola
chola
holache
     hola
"hola"
'"'"'hola'"'"'
hola'
    run_comparison_test '<<hola

hola'
    
    # Tests d'erreurs heredoc
    echo -e "${YELLOW}=== Tests erreurs heredoc ===${NC}"
    run_comparison_test "cat <<"
    run_comparison_test "echo hola <<<< bonjour"
    run_comparison_test "echo hola <<<<< bonjour"
    
    # Tests avec quotes et variables
    echo -e "${YELLOW}=== Tests quotes et variables ===${NC}"
    run_comparison_test "e'c'\"\"h\"\"o hola > bonjour"
    run_comparison_test "cat 'bo'\"\"n\"\"jour"
    run_comparison_test 'export HOLA="bonjour hello"'
    run_comparison_test '>$HOLA'
    run_comparison_test '>$HOLA>hey'
    run_comparison_test '>hey>$HOLA'
    
    # Tests de combinaisons complexes
    echo -e "${YELLOW}=== Tests combinaisons complexes ===${NC}"
    run_comparison_test "cat Makefile | cat -e | cat -e"
    run_comparison_test "cat Makefile | grep srcs | cat -e"
    run_comparison_test "cat Makefile | grep srcs | grep srcs | cat -e"
    run_comparison_test "export HOLA=bonjour | cat -e | cat -e"
    run_comparison_test "unset HOLA | cat -e"
    run_comparison_test "export | echo hola"
    
    # Tests avec des commandes longues
    echo -e "${YELLOW}=== Tests commandes longues ===${NC}"
    run_comparison_test "echo test.c | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e"
    run_comparison_test "ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls"
    
    
    # Tests avec sleep et time
    echo -e "${YELLOW}=== Tests sleep et time ===${NC}"
    run_comparison_test "sleep 1 | sleep 1"
    run_comparison_test "sleep 1 | exit"
    run_comparison_test "exit | sleep 1"

	echo -e "${YELLOW}=== Tests personnalisés ===${NC}"
	run_comparison_test "echo hola > > bonjour"
	run_comparison_test "echo hola < < bonjour"
	run_comparison_test "echo hola >>> bonjour"
	run_comparison_test "< pwd"
	run_comparison_test "< Makefile ."
	run_comparison_test "cat <pwd"
	run_comparison_test "cat <srcs/pwd"
	run_comparison_test "cat <../pwd"
	run_comparison_test "cat >>"
	run_comparison_test "cat >>>"
	run_comparison_test "cat >> <<"
	run_comparison_test "cat >> > >> << >>"
	run_comparison_test "cat < ls"
		


    # Nettoyage final
    echo -e "${YELLOW}=== Nettoyage final ===${NC}"
    echo "Suppression des fichiers créés pendant les tests..."
    rm -f bonjour hello hola hey a b c d e pwd ls1 ls2 hola1 hola2
    rm -f /tmp/bonjour /tmp/a /tmp/b /tmp/c
    rm -rf dir 2>/dev/null
    rm -f srcs/bonjour srcs/hello 2>/dev/null
    rm -f ../bonjour ../hello 2>/dev/null
    rm -f "bonjour hello" HOLA "bonjour 1" 2>/dev/null
    rm -f Docs/hey Docs/bonjour 2>/dev/null 
    rm -rf \"\"
    rm -rf \$\"HOLA\"
    rm -rf \$\"HOLA\"
    rm -rf \$\\HOLA\\
    rm -rf \\
    rm -rf \\\\
    echo "Tous les fichiers de test ont été supprimés."
}

show_report() {
    echo ""
    echo -e "${BLUE}=== RAPPORT FINAL ===${NC}"
    echo -e "Tests totaux: $TOTAL_TESTS"
    echo -e "${GREEN}Tests réussis: $PASSED_TESTS${NC}"
    echo -e "${RED}Tests échoués: $FAILED_TESTS${NC}"
    
    if [[ $FAILED_TESTS -eq 0 ]]; then
        echo -e "${GREEN}🎉 Tous les tests sont passés !${NC}"
        exit 0
    else
        local percentage=$((PASSED_TESTS * 100 / TOTAL_TESTS))
        echo -e "${YELLOW}Taux de réussite: $percentage%${NC}"
        exit 1
    fi
}

# Point d'entrée principal
main() {
    parse_args "$@"
    run_all_tests
    show_report
}

# Exécuter le script
main "$@"