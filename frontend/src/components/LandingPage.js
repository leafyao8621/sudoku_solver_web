import React, { useEffect, useState, useRef } from "react";
import axios from "axios";
import { json, Link, useLocation } from "react-router-dom";
import {
    Button,
    Form,
    FormGroup,
    Input,
    Label,
} from "reactstrap";

const LandingPage = (props) => {
    const location  = useLocation();
    const [blockRow, setBlockRow] = useState(3);
    const [blockColumn, setBlockColumn] = useState(3);
    const [selectedRow, setSelectedRow] = useState(0);
    const [selectedColumn, setSelectedColumn] = useState(0);
    const [data, setData] =
        useState(
            () => {
                return (
                    Array.from(
                        Array(9).keys()
                    ).map(
                        () => {
                            return (
                                Array.from(
                                    Array(9).keys()
                                ).map(() => null)
                            )
                        }
                    )
                );
            }
        );
    const solve = () => {
        const payload =
            {
                "block_row": blockRow,
                "block_column": blockColumn,
                "data": data
            }
        axios.post(
            "http://localhost:8000/solve",
            payload
        ).then((res) => {
            alert(res);
        }).catch((err) => {
            alert(err);
        })
    };
    const renderGrid = () => {
        return (
            <table>
                <tbody>
                {
                    Array.from(
                        Array(blockColumn * blockRow).keys()
                    ).map(
                        (i) => {
                            return (
                                <tr key={`inpr${i}`}>
                                    {
                                        Array.from(
                                            Array(blockColumn * blockRow).keys()
                                        ).map(
                                            (j) => {
                                                return (
                                                    <td key={`inp${i}${j}`}>
                                                        <Button
                                                            outline color=
                                                            {
                                                                i === selectedRow &&
                                                                j === selectedColumn ?
                                                                "warning" :
                                                                "primary"
                                                            }
                                                            onClick=
                                                            {
                                                                () => {
                                                                    setSelectedRow(i);
                                                                    setSelectedColumn(j);
                                                                }
                                                            }
                                                        >
                                                            {
                                                                data[i][j] ===
                                                                null ?
                                                                "X" :
                                                                data[i][j]
                                                            }
                                                        </Button>
                                                    </td>
                                                )
                                            }
                                        )
                                    }
                                </tr>
                            )
                        }
                    )
                }
                </tbody>
            </table>
        )
    };
    const renderInput = () => {
        return (
            <table>
                <tbody>
                {
                    Array.from(
                        Array(blockColumn * blockRow).keys()
                    ).map((i) => {
                        return (
                            <tr key={`contr${i}`}>
                                <td key={`cont${i}`}>
                                    <Button
                                        onClick=
                                        {
                                            () => {
                                                setData(
                                                    Array.from(
                                                        data.keys()
                                                    ).map((j) => {
                                                        return (
                                                            Array.from(
                                                                data[j].keys()
                                                            ).map((k) => {
                                                                return (
                                                                    j === selectedRow &&
                                                                    k === selectedColumn ?
                                                                    i + 1 :
                                                                    data[j][k]
                                                                );
                                                            })
                                                        );
                                                    })
                                                );
                                            }
                                        }
                                    >
                                        {i + 1}
                                    </Button>
                                </td>
                            </tr>
                        )
                    })
                }
                <tr>
                    <td>
                        <Button
                            onClick=
                            {
                                () => {
                                    setData(
                                        Array.from(
                                            data.keys()
                                        ).map((j) => {
                                            return (
                                                Array.from(
                                                    data[j].keys()
                                                ).map((k) => {
                                                    return (
                                                        j === selectedRow &&
                                                        k === selectedColumn ?
                                                        null :
                                                        data[j][k]
                                                    );
                                                })
                                            );
                                        })
                                    );
                                }
                            }
                        >
                            X
                        </Button>
                    </td>
                </tr>
                </tbody>
            </table>
        )
    };
    return (
        <main className="container">
            <h1>Logged in as { location.state.userName }</h1>
            <Form>
                <FormGroup>
                    <Label>Block Rows</Label>
                    <Input
                        type="number"
                        max="5"
                        min="1"
                        defaultValue="3"
                        onChange={({ target }) => {
                            const row = parseInt(target.value);
                            setBlockRow(row);
                            setData(
                                (
                                    () => {
                                        return (
                                            Array.from(
                                                Array(row * blockColumn).keys()
                                            ).map(
                                                () => {
                                                    return (
                                                        Array.from(
                                                            Array(
                                                                row *
                                                                blockColumn
                                                            ).keys()
                                                        ).map(() => null)
                                                    )
                                                }
                                            )
                                        );
                                    }
                                )()
                            )
                        }}
                    />
                    <Label>Block Columns</Label>
                    <Input
                        max="5"
                        min="1"
                        defaultValue="3"
                        onChange={({ target }) => {
                            setBlockColumn(target.value)
                        }}
                    />
                </FormGroup>
            </Form>
            <Button onClick={ solve }>Solve</Button>
            <table>
                <tbody>
                <tr>
                    <td>
                        { renderGrid() }
                    </td>
                    <td>
                        { renderInput() }
                    </td>
                </tr>
                </tbody>
            </table>
        </main>
    );
};

export default LandingPage;
