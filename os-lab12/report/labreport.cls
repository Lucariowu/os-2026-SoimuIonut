\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{labreport}[2026/02/09 Custom Lab Report Class]

% Base class
\LoadClass[12pt,a4paper]{article}

% Packages
\RequirePackage[utf8]{inputenc}
\RequirePackage[UKenglish]{babel} % UK English
\RequirePackage{geometry}
\RequirePackage{graphicx}
\RequirePackage{amsmath,amssymb}
\RequirePackage{listings}
\RequirePackage{xcolor}
\RequirePackage{hyperref}
\RequirePackage{booktabs}
\RequirePackage{float}
\RequirePackage{caption}
\RequirePackage{subcaption}
\RequirePackage{fancyhdr}
\RequirePackage{titlesec}
\RequirePackage{mathptmx} % Times New Roman font

% Page geometry defaults
\geometry{
    left=25mm,
    right=25mm,
    top=25mm,
    bottom=25mm,
    headheight=15pt
}

% Header and footer
\pagestyle{fancy}
\fancyhf{}
\rhead{\thepage}
\lhead{\leftmark}
\renewcommand{\headrulewidth}{0.4pt}

% Python code listing style
\lstdefinestyle{python}{
    language=Python,
    basicstyle=\ttfamily\small,
    keywordstyle=\color{blue}\bfseries,
    commentstyle=\color{gray}\itshape,
    stringstyle=\color{red},
    showstringspaces=false,
    breaklines=true,
    frame=single,
    numbers=left,
    numberstyle=\tiny\color{gray},
    backgroundcolor=\color{gray!10}
}

% Hyperref setup
\hypersetup{
    colorlinks=true,
    linkcolor=black,
    filecolor=magenta,      
    urlcolor=blue,
    citecolor=black
}

% Title page command
% #1 lab number, #2 title, #3 elaborated left, #4 elaborated right, #5 verified
\newcommand{\labtitle}[5]{%
    \begin{titlepage}
        \centering
        \large

        \includegraphics[width=3.6cm]{LOGO_UTM.jpg}\par
        \vspace{0.4cm}

        {\large\bfseries Ministerul Educației și Cercetării al Republicii Moldova\par}
        \vspace{0.25cm}
        {\LARGE\bfseries Universitatea Tehnică a Moldovei\par}
        \vspace{0.25cm}
        {\Large\bfseries Facultatea Calculatoare, Informatică și Microelectronică\par}
        \vspace{0.25cm}
        {\large\bfseries Departamentul Ingineria Software și Automatică\par}

        \vfill

        {\Large\bfseries Laboratory Work #1:\par}
        \vspace{0.4cm}
        {\Large #2\par}

        \vfill

        % ----- Elaborated & Verified -----
        \noindent
        \begin{tabular*}{\textwidth}{@{}l@{\extracolsep{\fill}}l@{}}
            \textbf{Elaborated:} & \begin{minipage}[t]{0.28\textwidth}\raggedright #3\\[0.15cm] #4\end{minipage} \\[0.7cm]
            \textbf{Verified:}   & \begin{minipage}[t]{0.28\textwidth}\raggedright #5\end{minipage}
        \end{tabular*}

        \vfill

        {\large Chișinău -- \the\year\par}

    \end{titlepage}
}
