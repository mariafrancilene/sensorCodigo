<?php
// Função para salvar a contagem em um arquivo
function salvarContagem($contagem) {
    // Caminho do arquivo onde a contagem será salva
    $arquivo = 'contagem.txt';

    // Escreve a contagem no arquivo
    file_put_contents($arquivo, $contagem);
}

// Função para ler a contagem do arquivo
function lerContagem() {
    // Caminho do arquivo onde a contagem é salva
    $arquivo = 'contagem.txt';

    // Lê a contagem do arquivo
    $contagem = file_get_contents($arquivo);

    return $contagem;
}

// Incrementa a contagem de detecções e salva no arquivo
function incrementarContagem() {
    // Lê a contagem atual do arquivo
    $contagemAtual = lerContagem();

    // Incrementa a contagem
    $novaContagem = intval($contagemAtual) + 1;

    // Salva a nova contagem no arquivo
    salvarContagem($novaContagem);

    return $novaContagem;
}

if(isset($_POST['minhaVariavelBooleana'])) {
    $minhaVariavelBooleana = $_POST['minhaVariavelBooleana'];

    // Verifica se a variável booleana é verdadeira
    if ($minhaVariavelBooleana) {
        // Incrementa a contagem de detecções e retorna a contagem atualizada
        echo incrementarContagem();
    } else {
        // Retorna a contagem atualizada
        echo lerContagem();
    }
} else {
    // Retorna a contagem atualizada
    echo lerContagem();
}
?>
