# Confi

The Confi DSL (Domain-Specific Language) is a simple language for defining configurations for various settings in a structured manner.

## Features

- Define configuration settings in sections and parameters.
- Organize settings into logical groups using sections.
- Specify values and descriptions for each parameter.

## Syntax

### Sections

Sections are used to group configuration settings. Syntax:

```confi
section <SectionName> {
// Configuration settings go here
}
```

### Parameters

Parameters define individual configuration settings within a section. Syntax:

parameter <ParameterName> {
value = <Value>
description = <Description>
}


### Example

Here's an example of a configuration using the Confi DSL:

```confi
section ServerConfiguration {
    parameter Hostname {
        value = "example.com"
        description = "The hostname of the server"
    }

    parameter Port {
        value = 8080
        description = "The port number to listen on"
    }
}
```
