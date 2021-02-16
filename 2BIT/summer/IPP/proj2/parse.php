<?php
/**
 * Project: Analyzátor kódu v IPPcode20
 * @file parse.php
 * @brief Script na analyzu kodu IPPcode20 a vygenerovanie jeho XML reprezentacie
 *
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 */

ini_set('display_errors', 'stderr');
require_once(__DIR__.'/Stats.php');
require_once(__DIR__.'/XMLDoc.php');
require_once(__DIR__.'/Errors.php');
require_once(__DIR__.'/Arguments.php');
require_once(__DIR__.'/Parser.php');

    try{
        // Proccessing arguments
        $statsObj = new Stats();
        $argsObj = new Arguments();
        $argsObj->checkArguments($statsObj);
        // header check
        $parserObj = new Parser();
        $xmldoc = new XMLDoc();
        $parserObj->headerCheck($xmldoc, $statsObj);
        // lex and syntax analysis
        $parserObj->analysis($xmldoc, $statsObj);
        // final print of XML representation and STATS
        $xmldoc->printDoc();
        $statsObj->printStats();
    } catch (Exception $e){
        exit($e->getCode());
    }
    exit(Errors::OK);
?>
